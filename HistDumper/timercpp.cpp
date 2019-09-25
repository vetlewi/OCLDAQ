//
// Created by Vetle Wegner Ingeberg on 25/09/2019.
//

#include "timercpp.h"

#include <thread>
#include <chrono>


void Timer::setTimeout(std::function<bool()> function, int delay) {
    this->clear = false;
    std::thread t([=]() {
        if(this->clear) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(this->clear) return;
        if ( !function() ) return;
    });
    t.detach();
}

void Timer::setInterval(std::function<bool()> function, int interval) {
    this->clear = false;
    std::thread t([=]() {
        while(true) {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(this->clear) return;
            if ( !function() ) return;
        }
    });
    t.detach();
}

void Timer::setInterval_CountDown(std::function<void(int)> function, int tot_num, int subinterval)
{
    this->clear = false;
    std::thread t([=]() {
        int have = 0;
        while(true) {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(subinterval));
            if(this->clear) return;
            function(tot_num-have);
            ++have;
        }
    });
    t.detach();
}

void Timer::stop() {
    this->clear = true;
}