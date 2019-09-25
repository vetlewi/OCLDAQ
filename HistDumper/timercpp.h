//
// Created by Vetle Wegner Ingeberg on 24/09/2019.
//

#ifndef TIMERCPP_H
#define TIMERCPP_H

#include <functional>

class Timer {
private:
    bool clear = false;

public:
    void setTimeout(std::function<bool()> function, int delay);
    void setInterval(std::function<bool()> function, int interval);
    void stop();

};

#endif // TIMERCPP_H
