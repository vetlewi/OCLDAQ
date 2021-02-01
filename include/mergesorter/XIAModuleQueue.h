//
// Created by Vetle Wegner Ingeberg on 11/12/2020.
//

#pragma once

#include <exception>
#include <queue>

namespace MergeSorter {

    template<size_t max_raw_size = 18>
    struct Event_t
    {
        struct Header_t
        {
            unsigned chanID: 4;
            unsigned slotID: 4;
            unsigned crateID: 4;
            unsigned headerLen: 5;
            unsigned eventLen: 14;
            bool finishCode: 1;
            unsigned event_ts_lo: 32;
            unsigned event_ts_hi: 16;
            unsigned cfd_result: 16;
            unsigned event_energy: 16;
            unsigned traceLen: 15;
            bool traceOutOfRange: 1;
        };

        constexpr static size_t header_size = sizeof(Header_t) / sizeof(uint32_t);

        int64_t timestamp;
        uint32_t raw[max_raw_size];
        size_t size;

        Event_t() : timestamp(0), raw{}, size(0) {}

        Event_t(const uint32_t *first, const size_t &len, const int &ts)
                : raw{}
        {
            if (len < header_size)
                throw std::length_error("First header missing");
            const auto *hdr = reinterpret_cast<const Header_t *>(first);
            if (len < hdr->headerLen)
                throw std::length_error("Not enough data to extract full header");
            if (hdr->headerLen > max_raw_size)
                throw std::length_error("Header too large");

            timestamp = (static_cast<int64_t>(hdr->event_ts_hi) << 32) | static_cast<int64_t>(hdr->event_ts_lo);
            timestamp *= ts;
            std::copy(first, first + hdr->headerLen, raw);
            reinterpret_cast<Header_t *>(raw)->eventLen = reinterpret_cast<Header_t *>(raw)->headerLen;
            size = hdr->headerLen;
        }
    };

    template<size_t s>
    inline bool operator>(const Event_t<s> &lhs, const Event_t<s> &rhs) { return lhs.timestamp > rhs.timestamp; }

    template<typename T>
    using Buffer_t = std::queue<T>;

    template<typename T>
    class XIAModuleQueue
    {
    private:

        Buffer_t<T> queue{};

        const int ts_factor;

    public:

        explicit XIAModuleQueue(const int &ts) : ts_factor( ts ){}

        void push(const uint32_t *data, const size_t &size)
        {
            size_t read = 0;
            while ( read < size ){
                queue.emplace(data+read, size, ts_factor);
                read += reinterpret_cast<const typename T::Header_t *>(data+read)->headerLen;
            }
        }

        inline const T &peek() const { return queue.front(); }

        inline T pop()
        {
            T top = peek();
            queue.pop();
            return top;
        }

        inline bool empty() const { queue.empty(); }
    };

    template<typename T>
    using Queue_t = std::priority_queue<T, std::vector<T>, std::greater<T>>;

    template<typename T>
    class queue_t
    {
    private:
        Queue_t<T> queue;
        size_t raw_size;

    public:

        inline const T &top() const { return queue.top(); }

        inline T pop()
        {
            T top = top();
            queue.pop();
            raw_size -= top.size;
            return top;
        }

        inline size_t size() const { return raw_size; }

        inline void push( const T &&value ){ queue.push(value); }

    };

    template<typename T>
    class XIAQueue
    {
    private:
        std::vector<XIAModuleQueue<T>> modules;
        queue_t<T> queue{};

        std::vector<uint32_t> overflow;

        const size_t min_readout;

    public:

        explicit XIAQueue(const std::vector<int> &ts_factors, const size_t &minreadout)
            : min_readout( minreadout )
        {
            for ( auto &factor : ts_factors ){
                modules.push_back(XIAModuleQueue<T>(factor));
            }
        }

        void fill(const uint32_t *raw, const size_t &size, const int &module)
        {
            modules[module].push(raw, size);
            while ( !modules[module].empty() ){
                queue.push(modules[module].pop());
            }
        }

        bool check_buffer(const size_t &size) const { return queue.size() < min_readout + size; }

        bool get_buffer(uint32_t *buffer, size_t bufsize, unsigned int *first_header)
        {
            size_t pos = 0;
            // First we will fill the data that got split
            for ( auto &data : overflow ){
                buffer[pos++] = data;
            }
            overflow.clear();
            *first_header = pos;

            T evt;
            while ( pos < bufsize ){
                evt = queue.top();
                if ( !(pos + evt.size < bufsize) ){
                    for ( size_t i = 0 ; i < evt.size ; ++i ){
                        if ( pos < bufsize )
                            buffer[pos++] = evt.raw[i];
                        else
                            overflow.push_back(evt.raw[i]);
                    }
                    continue;
                }
                std::copy(evt.raw, evt.raw+evt.size, buffer+pos);
                pos += evt.size;
            }

        }

    };

}