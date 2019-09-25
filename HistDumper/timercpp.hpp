//
// Created by Vetle Wegner Ingeberg on 24/09/2019.
//

#ifndef TIMERCPP_H
#define TIMERCPP_H

#include <iostream>
#include <thread>
#include <chrono>
#include <functional>

class Timer {
    bool clear = false;

public:
    void setTimeout(std::function<bool()> function, int delay);
    void setInterval(std::function<bool()> function, int interval);
    void stop();

};

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

void Timer::stop() {
    this->clear = true;
}

#endif // TIMERCPP_H
