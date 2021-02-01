//
// Created by Vetle Wegner Ingeberg on 11/12/2020.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdint>
#include <XIAModuleQueue.h>

#include <iostream>

TEST_CASE("Test creation of a single event"){
    uint32_t data[] = {540718, 360318980, 2013265920, 63};
    uint32_t data1[] = {540718, 360318980, 2013265920, 63};
    uint32_t data2[] = {540718, 360318980, 2013265920, 63};
    reinterpret_cast<MergeSorter::Event_t<10>::Header_t *>(data1)->event_ts_lo -= 2;
    reinterpret_cast<MergeSorter::Event_t<10>::Header_t *>(data2)->event_ts_lo *= 10/8 + 1;

    MergeSorter::Event_t<> event;

    CHECK_THROWS(event = MergeSorter::Event_t<>(data, 2, 10));
    CHECK_NOTHROW(event = MergeSorter::Event_t<>(data, 4, 10));


    MergeSorter::XIAModuleQueue<MergeSorter::Event_t<>> queue(10);
    CHECK_NOTHROW(queue.push(data, 4));

    std::vector<int> ts_mod = {10, 8};

    MergeSorter::XIAQueue<MergeSorter::Event_t<>> xiaqueue(ts_mod, 0);

    uint32_t res[12];
    std::cout << "Hello!" << std::endl;
    xiaqueue.fill(data, 4, 0);
    xiaqueue.fill(data1, 4, 0);
    xiaqueue.fill(data2, 4, 1);
    std::cout << "Hello!" << std::endl;
    const char *str = ( xiaqueue.check_buffer(12) ) ? "true" : "false";
    std::cout << str << std::endl;
    uint32_t tmp;
    xiaqueue.get_buffer(res, 12, &tmp);
}