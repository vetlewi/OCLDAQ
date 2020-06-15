//
// Created by Vetle Wegner Ingeberg on 15/06/2020.
//

#ifndef SCALERREADOUT_H
#define SCALERREADOUT_H

#include <chrono>

#include "ScalerTransmitter.h"

class ScalerReadout
{
private:

    //! Minimum interval between readouts
    std::chrono::milliseconds readout_interval;

    //! Last point in time where a readout occured
    std::chrono::high_resolution_clock::time_point last_readout;

    //! Transmitter class responsible for sending data to DB
    ScalerTransmitter transmitter;

public:

    //! Constructor
    ScalerReadout(const char *url = nullptr,
            std::chrono::milliseconds readout_interval = std::chrono::milliseconds(1000));

    //! Perform an actual readout
    void DoReadout();

};


#endif //SCALERREADOUT_H
