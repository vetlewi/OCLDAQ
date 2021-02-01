//
// Created by Vetle Wegner Ingeberg on 11/12/2020.
//

#include "XIAReadoutQueue.h"

struct XIA_base {
    unsigned chanID : 4;
    unsigned slotID : 4;
    unsigned crateID : 4;
    unsigned headerLen : 5;
    unsigned eventLen : 14;
    bool finishCode : 1;
    unsigned event_ts_lo : 32;
    unsigned event_ts_hi : 16;
    unsigned cfd_result : 16;
    unsigned event_energy : 16;
    unsigned traceLen : 15;
    bool traceOutOfRange : 1;
};

Event::Event(const uint32_t *start, const size_t &size, const int &ts)
{
    if ( size < 4 )
        throw std::length_error("Invalid header length, too short.");

    const auto *header = reinterpret_cast<const XIA_base *>(start);
    if ( header->headerLen > MAX_RAWDATA_LEN )
        throw std::length_error("Invalid header length, too long.");

    timestamp = (static_cast<int64_t>(header->event_ts_hi) << 32) | static_cast<int64_t>(header->event_ts_lo);
    timestamp *= ts;
    size_raw = header->headerLen;
    std::copy(start, start+header->headerLen, raw_data);
    reinterpret_cast<XIA_base *>(raw_data)->eventLen = reinterpret_cast<XIA_base *>(raw_data)->headerLen;
}


XIAReadoutQueue::XIAReadoutQueue(const std::vector<int> &ts_factor, const size_t &buf_size)
    : sorted_events( )
    , modules( 0 )
    , buffer_size( buf_size )
    , data_avalible( 0 )
    , most_recent_t{0}
    , timestamp_factor{0}
    , overflow_queue( )
    , overflow_fifo{}
{
    if ( !(ts_factor.size() <= PRESET_MAX_MODULES ) )
        throw std::length_error("Too many modules");
    overflow_queue.reserve(2*MAX_RAWDATA_LEN);
    for ( auto &ts : ts_factor ){
        timestamp_factor[modules++] = ts;
    }
}

void XIAReadoutQueue::process_buffer(const uint32_t *buffer, const size_t &size, const int &module)
{
    // We know (it should always be the case) that we start at the actual start of the event
    const XIA_base *header;
    size_t read = 0;
    while ( read < size ){
        header = reinterpret_cast<const XIA_base *>(buffer+read);
        if ( read + header->eventLen >= size ){
            overflow_fifo[module].insert(std::begin(overflow_fifo[module]), buffer+read, buffer+size);
            return; // Done!
        }
        sorted_events.emplace(buffer+read, size-read, timestamp_factor[module]);
        data_avalible += header->headerLen;
        read += header->eventLen;
    }
}

void XIAReadoutQueue::ParseQueue(const uint32_t *raw_data, const size_t &size, const int &module)
{
    if ( overflow_fifo[module].empty() ){
        process_buffer(raw_data, size, module);
    } else {
        std::vector<uint32_t> data;
        data.reserve(overflow_fifo[module].size() + size);
        data.insert(std::begin(data), std::begin(overflow_fifo[module]), std::end(overflow_fifo[module]));
        data.insert(std::end(data), raw_data, raw_data+size);
        overflow_fifo[module].clear();
        process_buffer(data.data(), data.size(), module);
    }
}

bool XIAReadoutQueue::check_buffer(const size_t &buffer_size)
{
    // Check if there is enough for a single buffer
    auto have = data_avalible + overflow_queue.size();
    if ( have < XIA_MIN_READOUT + buffer_size )
        return false;
    return true;
}

bool XIAReadoutQueue::fetch_buffer(uint32_t *buffer, size_t bufsize, unsigned int *first_header)
{
    if ( !check_buffer(bufsize) )
        return false;

    size_t pos = 0;

    // First we will fill the data that got split
    for ( auto &data : overflow_queue ){
        buffer[pos++] = data;
    }
    overflow_queue.clear();
    *first_header = pos;

    Event_t evt;
    while ( pos < bufsize ){
        evt = sorted_events.top();
        if ( !( pos + evt.size_raw < bufsize ) ){

        }
        std::copy(evt.raw_data, evt.raw_data+evt.size_raw, buffer+pos);
        pos +=
    }

}