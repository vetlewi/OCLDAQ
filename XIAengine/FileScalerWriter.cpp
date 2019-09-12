//
// Created by Vetle Wegner Ingeberg on 10/09/2019.
//

#include "FileScalerWriter.h"

#include <cstring>
#include <fstream>
#include <string>
#include <chrono>

#include "pixie16app_export.h"

uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

struct Rate
{
    double module_rate;
    double input_rate[NUMBER_OF_CHANNELS];
    double output_rate[NUMBER_OF_CHANNELS];
};

Module_Stats operator-(const Module_Stats &lhs, const Module_Stats &rhs)
{
    Module_Stats result;
    result.processed_events = lhs.processed_events - rhs.processed_events;
    result.real_time = lhs.real_time - rhs.real_time;
    for (int chan = 0 ; chan < NUMBER_OF_CHANNELS ; ++chan){
        result.live_time[chan] = lhs.live_time[chan] - rhs.live_time[chan];
        result.FastPeaks[chan] = lhs.FastPeaks[chan] - rhs.FastPeaks[chan];
        result.ChanEvents[chan] = lhs.ChanEvents[chan] - rhs.ChanEvents[chan];
    }
    return result;
}

Rate CalculateRate(const Module_Stats &lhs, const Module_Stats &rhs, const int &modADC)
{
    Module_Stats diff = lhs - rhs;
    Rate result;
    double realTime = 1.0e-6 * double(diff.real_time) / (double)SYSTEM_CLOCK_MHZ;
    double bgTime=0, liveTime;
    result.module_rate = ( realTime > 0 ) ? double(diff.processed_events)/realTime : 0;

    if ( modADC == 100 )
        bgTime = 1.0e-6 / 100.0;
    else if ( modADC == 250 )
        bgTime = 2.0 * 1.0e-6 / 250.0;
    else if ( modADC == 500 )
        bgTime = 5.0 * 1.0e-6 / 500.0;


    for (int ChanNum = 0 ; ChanNum < NUMBER_OF_CHANNELS ; ++ChanNum){
        liveTime = double( diff.live_time[ChanNum] )*bgTime;
        result.input_rate[ChanNum] = ( liveTime > 0 ) ? double(diff.FastPeaks[ChanNum])/liveTime : 0;
        result.output_rate[ChanNum] = ( realTime > 0 ) ? double(diff.ChanEvents[ChanNum])/realTime : 0;
    }

    return result;
}

FileScalerWriter::FileScalerWriter(const char *name)
    : ScalerWriter(  )
    , fname( new char[strlen(name)+1] )
{
    strcpy(fname, name);
}

FileScalerWriter::~FileScalerWriter()
{
    delete[] fname;
}


void FileScalerWriter::Transmit()
{
    std::ofstream outfile(fname, std::ios::out | std::ios::app);

    uint64_t time = timeSinceEpochMillisec();
    for (int modNum = 0 ; modNum < GetNumModules() ; ++modNum){
        outfile << time << ",";
        Rate rate = CalculateRate(scaler[modNum], pre_scaler[modNum], reinterpret_cast<Module_Info *>(GetModuleInfo())[modNum].Module_ADCMSPS);
        outfile << modNum;
        for (int chanNum = 0 ; chanNum < NUMBER_OF_CHANNELS ; ++chanNum){
            outfile << "," << std::scientific << rate.input_rate[chanNum];
            outfile << "," << std::scientific << rate.output_rate[chanNum];
        }
        outfile << "\n";
    }
}