//
// Created by Vetle Wegner Ingeberg on 15/06/2020.
//

#ifndef LISTMODEREADOUT_H
#define LISTMODEREADOUT_H

#include <vector>
#include <queue>
#include <chrono>
#include <algorithm>

typedef struct entry {
    long long timestamp;
    std::vector<unsigned int> raw_data;

    friend bool operator<(const entry &lhs, const entry &rhs);
    friend bool operator>(const entry &lhs, const entry &rhs);

    entry() : timestamp( 0 ), raw_data( 0 ){}

    entry(long long ts, const unsigned int *st, const size_t &si)
        : timestamp( ts )
        , raw_data( st, st+si ) {}

    inline size_t RawSize() const { return raw_data.size(); }

} entry_t;

inline bool operator<(const entry &lhs, const entry &rhs){
    return lhs.timestamp < rhs.timestamp;
}
inline bool operator>(const entry &lhs, const entry &rhs){
    return lhs.timestamp > rhs.timestamp;
}

template<typename T>
class General_Queue_t {

protected:

    //! Container implementing the buffer
    std::vector<T> buffer;

public:

    General_Queue_t() = default;

    explicit General_Queue_t(const size_t &min_size) : buffer( min_size ){}

    void push(T &&obj){ buffer.push_back(std::move(obj)); }

    [[nodiscard]] T &end() { return buffer.back(); }

    T &&pop(){
        std::sort(std::begin(buffer), std::end(buffer), std::greater<T>());
        T retval = std::move(buffer.back());
        buffer.pop_back();
        return retval;
    }

    inline void pop_back() { buffer.pop_back(); }

    [[nodiscard]] inline bool empty() const { return buffer.empty(); }

};

class Queue_t : public General_Queue_t<entry_t>
{
public :

    Queue_t() = default;

    explicit Queue_t(const size_t &min_size) : General_Queue_t(min_size){}

    [[nodiscard]] inline size_t RawSize() const {
        size_t size = 0;
        for ( auto &element : buffer ){
            size += element.RawSize();
        }
        return size;
    }
};

/*!
 * A class implementing the readout from the modules.
 * It contains all the logic required to check if there
 * is a need to read data from modules. It buffers the
 * data until it is sure that everything is in correct
 * time order.
 */
class ListModeReadout
{

private:

    //! Number of modules in the system
    unsigned short number_of_modules;

    //! Minimum number of words in buffer before a buffer can be read out
    size_t min_buffer;

    //! Minimum number of words in the XIA module FIFO before readout
    unsigned int min_fifo;

    //! Output buffer
    Queue_t queue;

    //! Scaling for timestamp
    int TSfactor[24 /*! Max number of XIA modules in one system */];

    //! Internal buffer
    unsigned int lmzdata[131072 /*! EXTERNAL_FIFO_LENGTH */];

    //! An entry can sometimes be split between two 'buffers'. We therefor need a event buffer
    std::vector<unsigned int> entry_buffer;

    //! Add data to the queue
    void AddData(const unsigned int* raw_data, const unsigned int& size, const int &module);

public:

    //! Constructor. Sets the min readout and the min
    //! buffer size.
    explicit ListModeReadout(const size_t& min_buffer = 65536,
            const unsigned int& min_fifo = 2048);

    //! Check if there is enough data in the buffer for a readout.
    //! Will return false current number of entries in the buffer
    //! is smaller than min_buffer + readout_len.
    bool check_buffer(const int& readout_len, bool end_of_run);

    //! Fetch data from buffer. Data is where the buffer should be copied to
    //! and length is the size of the buffer.
    bool fetch_buffer(unsigned int* data, const int& length, bool end_of_run);


};


#endif //OCLDAQ_LISTMODEREADOUT_H
