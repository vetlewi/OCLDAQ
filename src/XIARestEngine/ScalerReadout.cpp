//
// Created by Vetle Wegner Ingeberg on 15/06/2020.
//

#include "ScalerReadout.h"
#include "XIAinterface.h"

ScalerReadout::ScalerReadout(const char *url, std::chrono::milliseconds r_interval)
    : readout_interval( r_interval )
    , last_readout( std::chrono::high_resolution_clock::now() )
    , transmitter( ScalerTransmitter(url) )
{
}

void ScalerReadout::DoReadout()
{
    if ( std::chrono::high_resolution_clock::now() - last_readout < readout_interval )
        return;
    last_readout = std::chrono::high_resolution_clock::now();

    ScalerTransmitter::scaler_array_t array;
    ScalerTransmitter::scaler_t scalers;

    for ( unsigned short module = 0 ; module < XIAinterface::Get().Get_nModules() ; ++module ){
        auto retval = XIAinterface::Get().PixieReadStatisticsFromModule(array.data(), module);
        if ( retval < 0 ){
            std::string errmsg = "Pixie16ReadStatisticsFromModule failed, module = " + std::to_string(module);
            errmsg += ", retval = " + std::to_string(retval);
            throw std::runtime_error(errmsg);
        }
        scalers.push_back(array);
    }
    transmitter.ProcessScalers(scalers);
}