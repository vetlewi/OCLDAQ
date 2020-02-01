//
// Created by Vetle Wegner Ingeberg on 01/02/2020.
//

#include "XIAinterface.h"

#include <xia/pixie16app_export.h>

#define GUARD(func) int res; { std::lock_guard<std::mutex> guard(my_guard); res = func; } return res;

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

int XIAinterface::PixieBootModule(char *ComFPGAConfigFile, char *SPFPGAConfigFile, char *TrigFPGAConfigFile,
                                  char *DSPCodeFile, char *DSPParFile, char *DSPVarFile, unsigned short ModNum,
                                  unsigned short BootPattern)
{
    GUARD(Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile, TrigFPGAConfigFile, DSPCodeFile,
                            DSPParFile, DSPVarFile, ModNum, BootPattern))
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





