//
// Created by Vetle Wegner Ingeberg on 15/06/2020.
//

#include "ListModeReadout.h"

#include "XIAinterface.h"
#include "XIA_structures.h"

#include <spdlog/spdlog.h>

#define EXTFIFO_READ_THRESH 1024

ListModeReadout::ListModeReadout(const size_t &_min_buffer, const unsigned int &_min_fifo)
    : number_of_modules( XIAinterface::Get().Get_nModules() )
    , min_buffer( _min_buffer )
    , min_fifo( _min_fifo )
    , queue( 4*min_buffer )
    , TSfactor{ 10 }
{

    // Check info about all the modules to get TS factor
    unsigned short rev, adc_bits, msps;
    unsigned int serial;
    for ( int module = 0 ; module < number_of_modules ; ++module ){
        auto res = XIAinterface::Get().PixieReadModuleInfo(module, &rev, &serial, &adc_bits, &msps);
        if ( res < 0 ){
            spdlog::error("*ERROR* Pixie16ReadModuleInfo failed, retval = %d, module = %d", res, module);
            throw std::runtime_error("Pixie16ReadModuleInfo failed");
        }
        if ( msps == 100 || msps == 500 )
            TSfactor[module] = 10;
        else if ( msps == 250 )
            TSfactor[module] = 8;
        else {
            spdlog::warn("Unknown MSPS = %d in module %d, TS factor set to 10.", msps, module);
            TSfactor[module] = 10;
        }
    }
}

// ########################################################################

void ListModeReadout::AddData(const unsigned int* raw_data, const unsigned int &size, const int &module)
{
    int read = 0;
    const XIA_base *header;
    long long timestamp;
    while ( read < size ){
        header = reinterpret_cast<const XIA_base *>(raw_data + read);
        timestamp = static_cast<long long>(header->event_ts_hi) << 32 || // NOLINT(hicpp-signed-bitwise)
                static_cast<long long>(header->event_ts_lo); // NOLINT(hicpp-signed-bitwise)

        queue.push( {timestamp, raw_data, header->eventLen} );
        read += static_cast<int>(header->eventLen);
    }
}

// ########################################################################

bool ListModeReadout::check_buffer(const int &readout_len, bool end_of_run)
{
    // First we check if there is enough data to read from the modules
    int retval;
    unsigned int numFIFO;
    for ( unsigned short module = 0 ; module < number_of_modules ; ++module ){

        retval = XIAinterface::Get().PixieCheckExternalFIFOStatus(&numFIFO, module);
        if ( retval < 0 ){
            spdlog::error("*ERROR* Pixie16CheckExternalFIFOStatus failed, retval = %d", retval);
            return false;
        }

        if ( ( !end_of_run && numFIFO < EXTFIFO_READ_THRESH) || ( numFIFO > 0 ) ) // We do not read out this time
            continue;

        retval = XIAinterface::Get().PixieReadDataFromExternalFIFO(lmzdata, numFIFO, module);
        if ( retval < 0 ){
            spdlog::error("*ERROR* Pixie16ReadDataFromExternalFIFO failed, retval = %d", retval);
            return false;
        }

        // Add the data to the buffer
        AddData(lmzdata, numFIFO, module);
    }

    // Next we will check if the raw size of the buffer is large enough for buffer output.
    return ( queue.RawSize() + entry_buffer.size() - min_buffer >= readout_len );
}


// ########################################################################


bool ListModeReadout::fetch_buffer(unsigned int *data, const int &length, bool end_of_run)
{
    // First we check that there is in fact enough data to do a readout.
    // Is obsolete if check_buffer has been called properly first.
    if ( !end_of_run && queue.RawSize() + entry_buffer.size() - min_buffer < length )
        return false;

    unsigned int *current_pos = data;

    // If entrybuffer is larger than length, we have problems...
    if ( length < entry_buffer.size() ){
        spdlog::error("'data' is not large enough");
        return false;
    }

    // Copy the data from entry buffer to data (if we have data in the entry buffer)
    if ( !entry_buffer.empty() ) {
        std::copy(entry_buffer.begin(), entry_buffer.end(), current_pos);
        current_pos += entry_buffer.size();
        entry_buffer.clear();
    }

    while (current_pos < data+length){
        if ( ( end_of_run && queue.empty() ) ){
            // Set the rest of the buffer to zero
            std::fill(current_pos, data+length, 0);
            return true;
        } else if ( !end_of_run ) {
            return false;
        }

        if ( current_pos + queue.end().RawSize() < data + length ){
            std::copy(queue.end().raw_data.begin(), queue.end().raw_data.end(), current_pos);
            queue.pop_back();
            current_pos += queue.end().RawSize();
        } else {
            // Copy the data that we can to the buffer
            auto place_left = data+length - current_pos;
            std::copy(queue.end().raw_data.begin(), queue.end().raw_data.begin() + place_left, current_pos);
            entry_buffer.insert(entry_buffer.begin(), queue.end().raw_data.begin()+place_left, queue.end().raw_data.end());
            queue.pop_back();
            current_pos = data+length;
        }
    }
    return true;
}

// ########################################################################


