//
// Created by Vetle Wegner Ingeberg on 01/02/2020.
//

#include "XIAinterface.h"
#include "utils.h"

#include <xia/pixie16app_export.h>
#include <spdlog/spdlog.h>

#include <cmath>


#define GUARD(func) int res; { std::lock_guard<std::mutex> guard(my_guard); res = func; } return res;

XIAinterface::XIAinterface() : plxSlotMap( utils::ReadSlotMap() )
{
    // Initializing the system.
    int res = PixieInitSystem(static_cast<unsigned short>(plxSlotMap.size()),
            plxSlotMap.data(), 0);
    if (res < 0){
        std::string errmsg = "Pixie16InitSystem failed, retval = " + std::to_string(res);
        throw std::runtime_error(errmsg);
    }
    spdlog::info("Pixie-16 system successfully initialized");
}

int XIAinterface::PixieInitSystem(unsigned short NumModules, unsigned short *PXISlotMap, unsigned short OfflineMode)
{
    GUARD(Pixie16InitSystem(NumModules, PXISlotMap, OfflineMode))
}

int XIAinterface::PixieExitSystem(unsigned short ModNum)
{
    GUARD(Pixie16ExitSystem(ModNum))
}

int XIAinterface::PixieReadModuleInfo(unsigned short ModNum, unsigned short *ModRev, unsigned int *ModSerNum,
                                      unsigned short *ModADCBits, unsigned short *ModADCMSPS)
{
    GUARD(Pixie16ReadModuleInfo(ModNum, ModRev, ModSerNum, ModADCBits, ModADCMSPS))
}

int XIAinterface::PixieBootModule(const char *ComFPGAConfigFile, const char *SPFPGAConfigFile, const char *TrigFPGAConfigFile,
                                  const char *DSPCodeFile, const char *DSPParFile, const char *DSPVarFile, unsigned short ModNum,
                                  unsigned short BootPattern)
{
    char comFPGA[2048], spFPGA[2048], trigFPGA[2048], DSPcode[2048], DSPpar[2048], DSPvar[2048];
    snprintf(comFPGA, sizeof comFPGA, "%s", ComFPGAConfigFile);
    snprintf(spFPGA, sizeof spFPGA, "%s", SPFPGAConfigFile);
    snprintf(trigFPGA, sizeof trigFPGA, "%s", TrigFPGAConfigFile);
    snprintf(DSPcode, sizeof DSPcode, "%s", DSPCodeFile);
    snprintf(DSPpar, sizeof DSPpar, "%s", DSPParFile);
    snprintf(DSPvar, sizeof DSPvar, "%s", DSPVarFile);



    GUARD(Pixie16BootModule(comFPGA, spFPGA, trigFPGA, DSPcode,
                            DSPpar, DSPvar, ModNum, BootPattern))
}

int XIAinterface::PixieAcquireADCTrace(unsigned short ModNum)
{
    GUARD(Pixie16AcquireADCTrace(ModNum))
}

int XIAinterface::PixieReadSglChanADCTrace(unsigned short *Trace_Buffer, unsigned int Trace_Length,
        unsigned short ModNum, unsigned short ChanNum)
{
    GUARD(Pixie16ReadSglChanADCTrace(Trace_Buffer, Trace_Length, ModNum, ChanNum))
}

int XIAinterface::PixieIMbufferIO(unsigned int *Buffer, unsigned int NumWords, unsigned int Address,
                                  unsigned short Direction, unsigned short ModNum)
{
    GUARD(Pixie16IMbufferIO(Buffer, NumWords, Address, Direction, ModNum))
}

int XIAinterface::PixieEMbufferIO(unsigned int *Buffer, unsigned int NumWords, unsigned int Address,
                                  unsigned short Direction, unsigned short ModNum)
{
    GUARD(Pixie16EMbufferIO(Buffer, NumWords, Address, Direction, ModNum))
}

int XIAinterface::PixieStartListModeRun(unsigned short ModNum, unsigned short RunType, unsigned short mode)
{
    GUARD(Pixie16StartListModeRun(ModNum, RunType, mode))
}

int XIAinterface::PixieStartHistogramRun(unsigned short ModNum, unsigned short mode)
{
    GUARD(Pixie16StartHistogramRun(ModNum, mode))
}

int XIAinterface::PixieCheckRunStatus(unsigned short ModNum)
{
    GUARD(Pixie16CheckRunStatus(ModNum))
}

int XIAinterface::PixieEndRun(unsigned short ModNum)
{
    GUARD(Pixie16EndRun(ModNum))
}

double XIAinterface::PixieComputeInputCountRate(unsigned int *Statistics,
                                             unsigned short ModNum,
                                             unsigned short ChanNum)
{
    GUARD(Pixie16ComputeInputCountRate(Statistics, ModNum, ChanNum))
}

double XIAinterface::PixieComputeOutputCountRate(unsigned int *Statistics,
                                                unsigned short ModNum,
                                                unsigned short ChanNum)
{
    GUARD(Pixie16ComputeOutputCountRate(Statistics, ModNum, ChanNum))
}

double XIAinterface::PixieComputeLiveTime(unsigned int *Statistics, unsigned short ModNum, unsigned short ChanNum)
{
    GUARD(Pixie16ComputeLiveTime(Statistics, ModNum, ChanNum))
}

double XIAinterface::PixieComputeProcessedEvents(
        unsigned int *Statistics,
        unsigned short ModNum)
{
    GUARD(Pixie16ComputeProcessedEvents(Statistics, ModNum))
}

double XIAinterface::PixieComputeRealTime(
        unsigned int *Statistics,
        unsigned short ModNum)
{
    GUARD(Pixie16ComputeRealTime(Statistics, ModNum))
}

int XIAinterface::PixiecomplexFFT(
        double *data,
        unsigned int length)
{
    GUARD(Pixie16complexFFT(data, length))
}

unsigned short XIAinterface::APP16_TstBit(unsigned short bit, unsigned short value)
{
    return(((value & (unsigned short)(pow(2.0, (double)bit))) >> bit));
}

unsigned short XIAinterface::APP16_SetBit(unsigned short bit, unsigned short value)
{
    return(value | (unsigned short)(pow(2.0, (double)bit)));
}

unsigned short XIAinterface::APP16_ClrBit(unsigned short bit, unsigned short value)
{
    value = APP16_SetBit(bit, value);
    return(value ^ (unsigned short)(pow(2.0, (double)bit)));
}

unsigned int XIAinterface::APP32_TstBit(unsigned short bit, unsigned int value)
{
    return(((value & (unsigned int)(pow(2.0, (double)bit))) >> bit));
}

unsigned int XIAinterface::APP32_SetBit(unsigned short bit, unsigned int value)
{
    return(value | (unsigned int)(pow(2.0, (double)bit)));
}

unsigned int XIAinterface::APP32_ClrBit(unsigned short bit, unsigned int value)
{
    value = APP32_SetBit(bit, value);
    return(value ^ (unsigned int)(pow(2.0, (double)bit)));
}


int XIAinterface::PixieSetDACs(unsigned short ModNum)
{
    GUARD(Pixie16SetDACs(ModNum))
}

int XIAinterface::PixieProgramFippi(unsigned short ModNum)
{
    GUARD(Pixie16ProgramFippi(ModNum))
}

int XIAinterface::PixieAdjustOffsets(unsigned short ModNum)
{
    GUARD(PixieAdjustOffsets(ModNum))
}

int XIAinterface::PixieAcquireBaselines(unsigned short ModNum)
{
    GUARD(Pixie16AcquireBaselines(ModNum))
}

int XIAinterface::PixieReadSglChanBaselines(double *Baselines, double *TimeStamps, unsigned short NumBases,
                                            unsigned short ModNum, unsigned short ChanNum)
{
    GUARD(Pixie16ReadSglChanBaselines(Baselines, TimeStamps, NumBases, ModNum, ChanNum))
}

int XIAinterface::PixieRampOffsetDACs(double *DCValues, unsigned short NumDCVals, unsigned short ModNum)
{
    GUARD(Pixie16RampOffsetDACs(DCValues, NumDCVals, ModNum))
}

int XIAinterface::PixieControlTaskRun(unsigned short ModNum, unsigned short ControlTask, unsigned int Max_Poll)
{
    GUARD(Pixie16ControlTaskRun(ModNum, ControlTask, Max_Poll))
}

int XIAinterface::PixieBLcutFinder(unsigned short ModNum, unsigned short ChanNum, unsigned int *BLcut)
{
    GUARD(Pixie16BLcutFinder(ModNum, ChanNum, BLcut))
}

int XIAinterface::PixieTauFinder(unsigned short ModNum, double *Tau)
{
    GUARD(Pixie16TauFinder(ModNum, Tau))
}

int XIAinterface::PixieWriteSglModPar(char *ModParName, unsigned int ModParData, unsigned short ModNum)
{
    GUARD(Pixie16WriteSglModPar(ModParName, ModParData, ModNum))
}

int XIAinterface::PixieReadSglModPar(char *ModParName, unsigned int *ModParData, unsigned short ModNum)
{
    GUARD(Pixie16ReadSglModPar(ModParName, ModParData, ModNum))
}

int XIAinterface::PixieWriteSglChanPar(char *ChanParName, double ChanParData, unsigned short ModNum,
                                       unsigned short ChanNum)
{
    GUARD(Pixie16WriteSglChanPar(ChanParName, ChanParData, ModNum, ChanNum))
}

int XIAinterface::PixieReadSglChanPar(char *ChanParName, double *ChanParData, unsigned short ModNum,
                                      unsigned short ChanNum)
{
    GUARD(Pixie16ReadSglChanPar(ChanParName, ChanParData, ModNum, ChanNum))
}

int XIAinterface::PixieReadHistogramFromModule(unsigned int *Histogram, unsigned int NumWords, unsigned short ModNum,
                                               unsigned short ChanNum)
{
    GUARD(Pixie16ReadHistogramFromModule(Histogram, NumWords, ModNum, ChanNum))
}

int XIAinterface::PixieReadStatisticsFromModule(unsigned int *Statistics, unsigned short ModNum)
{
    GUARD(Pixie16ReadStatisticsFromModule(Statistics, ModNum))
}

int XIAinterface::PixieSaveHistogramToFile(char *FileName, unsigned short ModNum)
{
    GUARD(Pixie16SaveHistogramToFile(FileName, ModNum))
}

int XIAinterface::PixieGetModuleEvents(char *FileName, unsigned int *ModuleEvents)
{
    GUARD(Pixie16GetModuleEvents(FileName, ModuleEvents))
}

int XIAinterface::PixieGetEventsInfo(char *FileName, unsigned int *EventInformation, unsigned short ModuleNumber)
{
    GUARD(Pixie16GetEventsInfo(FileName, EventInformation, ModuleNumber))
}

int XIAinterface::PixieReadListModeTrace(char *FileName, unsigned short *Trace_Data, unsigned short NumWords,
                                         unsigned int FileLocation)
{
    GUARD(Pixie16ReadListModeTrace(FileName, Trace_Data, NumWords, FileLocation))
}

int XIAinterface::PixieReadHistogramFromFile(char *FileName, unsigned int *Histogram, unsigned int NumWords,
                                             unsigned short ModNum, unsigned short ChanNum)
{
    GUARD(Pixie16ReadHistogramFromFile(FileName, Histogram, NumWords, ModNum, ChanNum))
}

int XIAinterface::PixieSaveDSPParametersToFile(char *FileName)
{
    GUARD(Pixie16SaveDSPParametersToFile(FileName))
}

int XIAinterface::PixieLoadDSPParametersFromFile(char *FileName)
{
    GUARD(Pixie16LoadDSPParametersFromFile(FileName))
}

int
XIAinterface::PixieCopyDSPParameters(unsigned short BitMask, unsigned short SourceModule, unsigned short SourceChannel,
                                     unsigned short *DestinationMask)
{
    GUARD(Pixie16CopyDSPParameters(BitMask, SourceChannel, SourceChannel, DestinationMask))
}

int XIAinterface::PixieReadMSGFile(char *ReturnMsgStr)
{
    GUARD(Pixie16ReadMSGFile(ReturnMsgStr))
}

unsigned int XIAinterface::Decimal2IEEEFloating(double DecimalNumber)
{
    unsigned int IEEEFloatingNum, IntPart, IntPartHex, FractPartHex;
    double AbsDecNum, FractPart;
    unsigned short k, m, exponent;
    short n;
    char signbit;

    // Check signbit
    if(DecimalNumber > 0)
    {
        signbit = 0;
    }
    else if(DecimalNumber < 0)
    {
        signbit = 1;
    }
    else	// DecimalNumber = 0
    {
        return(0);
    }

    // Get the absolute value
    AbsDecNum = fabs(DecimalNumber);

    // Get the integer part
    IntPart = (unsigned int)floor(AbsDecNum);

    // Convert the integer part
    IntPartHex = 0;
    k = 0;
    do
    {
        if((IntPart % 2) == 1)
        {
            IntPartHex = APP32_SetBit(k, IntPartHex);
        }
        else
        {
            IntPartHex = APP32_ClrBit(k, IntPartHex);
        }

        k ++;

        IntPart /= 2;

    } while((IntPart > 0) && (k < 32));

    // Get the fractional part
    FractPart = AbsDecNum - floor(AbsDecNum);

    // Convert the fractional part
    FractPartHex = 0;
    m = 0;
    do
    {
        if((FractPart * 2.0) >= 1.0)
        {
            FractPartHex = APP32_SetBit((unsigned short)(31 - m), FractPartHex);
            FractPart = FractPart * 2.0 - 1.0;
        }
        else
        {
            FractPartHex = APP32_ClrBit((unsigned short)(31 - m), FractPartHex);
            FractPart = FractPart * 2.0;
        }

        m ++;

    } while((FractPart > 0) && (m < 32));

    // Combine the integer and fractional part
    // First, find the exponent
    if(IntPartHex > 0)
    {
        exponent = 127 + k - 1;	// 127 is the exponent bias

        // Add sign bit first
        IEEEFloatingNum = signbit * (unsigned int)pow(2.0, 31.0);

        // Add the exponent
        IEEEFloatingNum += exponent * (unsigned int)pow(2.0, 23.0);

        // Add the mantissa
        IntPartHex = APP32_ClrBit((unsigned short)(k - 1), IntPartHex);
        IEEEFloatingNum += IntPartHex * (unsigned int)pow(2.0, (double)(22 - (k - 2)));
        IEEEFloatingNum += FractPartHex >> (32 - (23 - (k - 1)));
    }
    else	// IntPartHex = 0
    {
        // Search the fist non-zero bit in FractPartHex
        for(n = 31; n >= 0; n --)
        {
            if(APP32_TstBit(n, FractPartHex) == 1)
            {
                break;
            }
        }

        exponent = 127 - (32 - n);	// 127 is the exponent bias

        // Add sign bit first
        IEEEFloatingNum = signbit * (unsigned int)pow(2.0, 31.0);

        // Add the exponent
        IEEEFloatingNum += exponent * (unsigned int)pow(2.0, 23.0);

        // Add the mantissa
        FractPartHex = APP32_ClrBit(n, FractPartHex);
        if(n >= 23)
        {
            IEEEFloatingNum += FractPartHex >> (n - 23);
        }
        else
        {
            IEEEFloatingNum += FractPartHex << (23 - n);
        }
    }

    return(IEEEFloatingNum);
}

double XIAinterface::IEEEFloating2Decimal(unsigned int IEEEFloatingNumber)
{
    short signbit, exponent;
    double mantissa, DecimalNumber;

    signbit = (short)(IEEEFloatingNumber >> 31);
    exponent = (short)((IEEEFloatingNumber & 0x7F800000) >> 23) - 127;
    mantissa = 1.0 + (double)(IEEEFloatingNumber & 0x7FFFFF) / pow(2.0, 23.0);
    if(signbit == 0)
    {
        DecimalNumber = mantissa * pow(2.0, (double)exponent);
    }
    else
    {
        DecimalNumber = - mantissa * pow(2.0, (double)exponent);
    }

    return(DecimalNumber);
}

int XIAinterface::PixieSaveExternalFIFODataToFile(char *FileName, unsigned int *nFIFOWords, unsigned short ModNum,
                                                  unsigned short EndOfRunRead)
{
    GUARD(Pixie16SaveExternalFIFODataToFile(FileName, nFIFOWords, ModNum, EndOfRunRead))
}

int XIAinterface::PixieRegisterIO(unsigned short ModNum, unsigned int address, unsigned short direction,
                                  unsigned int *value)
{
    GUARD(Pixie16RegisterIO(ModNum, address, direction, value))
}

int XIAinterface::PixieReadCSR(unsigned short ModNum, unsigned int *CSR)
{
    GUARD(Pixie16ReadCSR(ModNum, CSR))
}

int XIAinterface::PixieWriteCSR(unsigned short ModNum, unsigned int CSR)
{
    GUARD(Pixie16WriteCSR(ModNum, CSR))
}

int XIAinterface::PixieCheckExternalFIFOStatus(unsigned int *nFIFOWords, unsigned short ModNum)
{
    GUARD(Pixie16CheckExternalFIFOStatus(nFIFOWords, ModNum))
}

int
XIAinterface::PixieReadDataFromExternalFIFO(unsigned int *ExtFIFO_Data, unsigned int nFIFOWords, unsigned short ModNum)
{
    GUARD(Pixie16ReadDataFromExternalFIFO(ExtFIFO_Data, nFIFOWords, ModNum))
}









