//
// Created by Vetle Wegner Ingeberg on 11/12/2020.
//

#ifndef OCLDAQ_XIAREADOUTQUEUE_H
#define OCLDAQ_XIAREADOUTQUEUE_H

#include <queue>
#include <vector>

#include <cstdint>

#include <blockingconcurrentqueue.h>

#ifndef MAX_RAWDATA_LEN
#define MAX_RAWDATA_LEN 18 // Maximum length of a raw event with trace
#endif // MAX_RAWDATA_LEN

#ifndef XIA_MIN_READOUT
#define XIA_MIN_READOUT 65536
#endif // XIA_MIN_READOUT

#ifndef PRESET_MAX_MODULES
#define PRESET_MAX_MODULES 24
#endif // PRESET_MAX_MODULES

struct Event {
    int64_t timestamp;                  //! Timestamp of the event.
    uint32_t raw_data[MAX_RAWDATA_LEN]; //! Pointer to the raw data.
    size_t size_raw;                    //! Size of the raw data in number of 32 bit words.

    Event() = default;
    Event(const uint32_t *start, const size_t &length, const int &ts);
};
typedef struct Event Event_t;
inline bool operator>(const Event_t &a, const Event_t &b) { return (a.timestamp>b.timestamp); }

using queue_t = std::priority_queue<Event_t, std::vector<Event_t>, std::greater<Event_t> >;

/*!
 * A dedicated queue for XIA data.
 */
class XIAReadoutQueue
{
private:

    // Container for the queue of data
    queue_t sorted_events;

    // Number of modules in the system
    size_t modules;

    // Queue buffer wait size
    size_t buffer_size;

    // Number of 32-bit words in the queue
    size_t data_avalible;

    // Most recent timestamp of each module
    int64_t most_recent_t[PRESET_MAX_MODULES];

    // Factor to determine the time stamp factor (if 250MHz = 8, if 500 = 10).
    int timestamp_factor[PRESET_MAX_MODULES];

    // Temporary storage for data that are trapped between two buffers.
    std::vector<uint32_t> overflow_queue;

    // Temporary storage for data that are trapped between two buffers.
    std::vector<uint32_t> overflow_fifo[PRESET_MAX_MODULES];

    void process_buffer(const uint32_t *buffer, const size_t &size, const int &module);

public:

    // Initialize the queue
    explicit XIAReadoutQueue(const std::vector<int> &ts_factor, const size_t &buffer_size = XIA_MIN_READOUT);

    // Add data to the queue
    void ParseQueue(const uint32_t *raw_data, const size_t &size, const int &module);

    // Check if a buffer is ready for readout
    bool check_buffer(const size_t &buffer_size);

    // Get a buffer
    bool fetch_buffer(uint32_t *buffer, size_t bufsize, unsigned int *first_header);


};


#endif //OCLDAQ_XIAREADOUTQUEUE_H
