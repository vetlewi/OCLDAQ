//
// Created by Vetle Wegner Ingeberg on 01/02/2020.
//

#ifndef OCLDAQ_XIAINTERFACE_H
#define OCLDAQ_XIAINTERFACE_H

#include <mutex>


/*!
 * A class to wrap all XIA user functions.
 */
class XIAinterface
{


private:

    //! Mutex to lock any access to XIA API
    std::mutex my_guard;

public:

    int PixieInitSystem(
            unsigned short NumModules,
            unsigned short *PXISlotMap,
            unsigned short OfflineMode
    );

    int PixieExitSystem(unsigned short ModNum);

    int PixieReadModuleInfo(
            unsigned short ModNum,      /*!< module number */
            unsigned short *ModRev,     /*!< returned module revision */
            unsigned int *ModSerNum,  /*!< returned module serial number */
            unsigned short *ModADCBits, /*!< returned module ADC bits */
            unsigned short *ModADCMSPS  /*!< returned module ADC sampling rate */);

    int PixieBootModule(
            char *ComFPGAConfigFile,      // name of communications FPGA configuration file
            char *SPFPGAConfigFile,       // name of signal processing FPGA configuration file
            char *TrigFPGAConfigFile,     // name of trigger FPGA configuration file
            char *DSPCodeFile,            // name of executable code file for digital signal processor (DSP)
            char *DSPParFile,             // name of DSP parameter file
            char *DSPVarFile,             // name of DSP variable names file
            unsigned short ModNum,        // pixie module number
            unsigned short BootPattern); // boot pattern bit mask

    int PixieAcquireADCTrace(
            unsigned short ModNum);      // module number

    int PixieReadSglChanADCTrace(
            unsigned short *Trace_Buffer, // trace data
            unsigned int Trace_Length,    // trace length
            unsigned short ModNum,        // module number
            unsigned short ChanNum);     // channel number

    int PixieIMbufferIO(
            unsigned int *Buffer,         // buffer data
            unsigned int NumWords,        // number of buffer words to read or write
            unsigned int Address,         // buffer address
            unsigned short Direction,     // I/O direction
            unsigned short ModNum);      // module number

    int PixieEMbufferIO(
            unsigned int *Buffer,         // buffer data
            unsigned int NumWords,        // number of buffer words to read or write
            unsigned int Address,         // buffer address
            unsigned short Direction,     // I/O direction
            unsigned short ModNum);      // module number

    int PixieStartListModeRun(
            unsigned short ModNum,        // module number
            unsigned short RunType,       // run type
            unsigned short mode);        // run mode

    int PixieStartHistogramRun(
            unsigned short ModNum,        // module number
            unsigned short mode);        // run mode

    int PixieCheckRunStatus(
            unsigned short ModNum);      // Pixie module number

    int PixieEndRun(
            unsigned short ModNum);      // Pixie module number

    double PixieComputeInputCountRate(
            unsigned int *Statistics,
            unsigned short ModNum,
            unsigned short ChanNum);

    double PixieComputeOutputCountRate(
            unsigned int *Statistics,
            unsigned short ModNum,
            unsigned short ChanNum);

    double PixieComputeLiveTime(
            unsigned int *Statistics,
            unsigned short ModNum,
            unsigned short ChanNum);

    double PixieComputeProcessedEvents(
            unsigned int *Statistics,
            unsigned short ModNum);

    double PixieComputeRealTime(
            unsigned int *Statistics,
            unsigned short ModNums);

    int PixiecomplexFFT(
            double *data,
            unsigned int length);

    unsigned short APP16_TstBit(
            unsigned short bit,
            unsigned short value);

    unsigned short APP16_SetBit(
            unsigned short bit,
            unsigned short value);

    unsigned short APP16_ClrBit(
            unsigned short bit,
            unsigned short value);

    unsigned int APP32_SetBit(
            unsigned short bit,
            unsigned int value);

    unsigned int APP32_ClrBit(
            unsigned short bit,
            unsigned int value);

    unsigned int APP32_TstBit(
            unsigned short bit,
            unsigned int value);

    int PixieSetDACs(
            unsigned short ModNum);

    int PixieProgramFippi(
            unsigned short ModNum);

    int PixieAdjustOffsets(
            unsigned short ModNum);

    int PixieAcquireBaselines(
            unsigned short ModNum);      // module number

    int PixieReadSglChanBaselines(
            double *Baselines,            // returned baselines values
            double *TimeStamps,           // time stamp for each baseline value
            unsigned short NumBases,      // number of baseline values to read
            unsigned short ModNum,        // module number
            unsigned short ChanNum);     // channel number

    int PixieRampOffsetDACs(
            double *DCValues,             // returned DC offset values
            unsigned short NumDCVals,     // number of DC values to read
            unsigned short ModNum);

    int PixieControlTaskRun(
            unsigned short ModNum,        // Pixie module number
            unsigned short ControlTask,   // Control task number
            unsigned int Max_Poll);    // Timeout control in unit of ms for control task run

    int PixieBLcutFinder(
            unsigned short ModNum,        // Pixie module number
            unsigned short ChanNum,       // Pixie channel number
            unsigned int *BLcut);      // BLcut return value

    int PixieTauFinder(
            unsigned short ModNum,        // Pixie module number
            double *Tau);        // 16 returned Tau values, in µs

    int PixieWriteSglModPar(
            char *ModParName,             // the name of the module parameter
            unsigned int ModParData,    // the module parameter value to be written to the module
            unsigned short ModNum);      // module number

    int PixieReadSglModPar(
            char *ModParName,             // the name of the module parameter
            unsigned int *ModParData,   // the module parameter value to be read from the module
            unsigned short ModNum);      // module number

    int PixieWriteSglChanPar(
            char *ChanParName,            // the name of the channel parameter
            double ChanParData,           // the channel parameter value to be written to the module
            unsigned short ModNum,        // module number
            unsigned short ChanNum);     // channel number

    int PixieReadSglChanPar(
            char *ChanParName,            // the name of the channel parameter
            double *ChanParData,          // the channel parameter value to be read from the module
            unsigned short ModNum,        // module number
            unsigned short ChanNum);     // channel number

    int PixieReadHistogramFromModule(
            unsigned int *Histogram,    // histogram data
            unsigned int NumWords,      // number of words to be read out
            unsigned short ModNum,        // module number
            unsigned short ChanNum);      // channel number

    int PixieReadStatisticsFromModule(
            unsigned int *Statistics,   // run statistics data
            unsigned short ModNum);      // module number

    int PixieSaveHistogramToFile(
            char *FileName,               // histogram data file name
            unsigned short ModNum);       // module number

    int PixieGetModuleEvents(
            char *FileName,               // the list mode data file name (with complete path)
            unsigned int *ModuleEvents); // receives number of events for each module

    int PixieGetEventsInfo(
            char *FileName,                   // the list mode data file name (with complete path)
            unsigned int *EventInformation, // to hold event information
            unsigned short ModuleNumber);     // the module whose events are to be retrieved

    int PixieReadListModeTrace(
            char *FileName,               // list mode data file name
            unsigned short *Trace_Data,   // list mode trace data (16-bit words)
            unsigned short NumWords,      // number of 16-bit words to be read out
            unsigned int FileLocation); // the location of the trace in the file

    int PixieReadHistogramFromFile(
            char *FileName,              // the histogram data file name (with complete path)
            unsigned int *Histogram,      // histogram data
            unsigned int NumWords,        // number of words to be read out
            unsigned short ModNum,        // module number
            unsigned short ChanNum);      // channel number

    int PixieSaveDSPParametersToFile(
            char *FileName);            // the DSP parameters file name (with complete path)

    int PixieLoadDSPParametersFromFile(
            char *FileName);            // the DSP parameters file name (with complete path)

    int PixieCopyDSPParameters(
            unsigned short BitMask,             // copy items bit mask
            unsigned short SourceModule,        // source module
            unsigned short SourceChannel,       // source channel
            unsigned short *DestinationMask);  // the destination module and channel bit mask

    int PixieReadMSGFile(
            char *ReturnMsgStr);

    unsigned int Decimal2IEEEFloating(double DecimalNumber);

    double IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);

    int PixieSaveExternalFIFODataToFile(
            char *FileName,               // list mode data file name
            unsigned int *nFIFOWords,   // number of words read from external FIFO
            unsigned short ModNum,        // module number
            unsigned short EndOfRunRead); // indicator whether this is the end of run read

    int PixieRegisterIO(
            unsigned short ModNum,        // the Pixie module to communicate to
            unsigned int address,       // register address
            unsigned short direction,     // either MOD_READ or MOD_WRITE
            unsigned int *value);      // holds or receives the data

    int PixieReadCSR(
            unsigned short ModNum,
            unsigned int *CSR);

    int PixieWriteCSR(
            unsigned short ModNum,
            unsigned int CSR);

    int PixieCheckExternalFIFOStatus(
            unsigned int *nFIFOWords,
            unsigned short ModNum);

    int PixieReadDataFromExternalFIFO(
            unsigned int *ExtFIFO_Data, // To receive the external FIFO data
            unsigned int nFIFOWords,    // number of words to read from external FIFO
            unsigned short ModNum);      // module number
};


#endif //OCLDAQ_XIAINTERFACE_H
