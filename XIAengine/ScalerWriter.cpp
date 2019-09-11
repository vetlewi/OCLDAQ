//
// Created by Vetle Wegner Ingeberg on 11/09/2019.
//

#include "ScalerWriter.h"

#include "pixie16app_export.h"

#include <cstring>

ScalerWriter::ScalerWriter()
    : pre_scaler( nullptr )
    , scaler(reinterpret_cast<Module_Stats *>(GetStatistics()) )
{
}

ScalerWriter::~ScalerWriter()
{
    Clear();
}

void ScalerWriter::Clear()
{
    if ( pre_scaler != nullptr )
        delete[] pre_scaler;
    pre_scaler = nullptr;
}

int ScalerWriter::AcquireScalers()
{
    int ret;
    if ( pre_scaler == nullptr ){
        ret = Pixie16UpdateStatistics();
        pre_scaler = new Module_Stats[PRESET_MAX_MODULES];
        return ret;
    }
    memcpy(pre_scaler, scaler, sizeof(Module_Stats)*PRESET_MAX_MODULES);
    ret = Pixie16UpdateStatistics();
    if ( ret < 0 )
        return ret;

    Transmit();
    return 0;

}