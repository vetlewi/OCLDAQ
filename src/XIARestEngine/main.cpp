//
// Created by Vetle Wegner Ingeberg on 12/06/2020.
//

#include "XIAinterface.h"
#include "FirmwareMapper.h"
#include "ListModeReadout.h"
#include "ScalerReadout.h"

#include <iostream>

#include <cstdio>

#include <cereal/types/map.hpp>
#include <cereal/archives/json.hpp>
#include <httplib.h>
#include <jthread.hpp>
#include <spdlog/spdlog.h>

#define BUFFER_SIZE 32768

inline bool LMReadout_loop(FILE *file, unsigned int *buffer, ListModeReadout &readout, const bool &eor)
{
    // We will read data anyways... But if the file is != null then we will
    // write to that file.
    // Also there will be a readout class that are responsible for all readouts.
    if ( readout.check_buffer(BUFFER_SIZE, eor) ){
        if ( !readout.fetch_buffer(buffer, BUFFER_SIZE, eor) ){
            throw std::runtime_error("Error fetching buffer");
        }
        return true;
    } else {
        return false;
    }
}

bool bootXIA(const firmware_map_t &fw_map, const char *settings)
{
    auto &interface = XIAinterface::Get();
    unsigned short rev, adc_bits, msps;
    unsigned int serial;
    for ( int module = 0 ; module < interface.Get_nModules() ; ++module ){
        auto retval = interface.PixieReadModuleInfo(module, &rev, &serial, &adc_bits, &msps);
        if ( retval < 0 ){
            spdlog::error("Pixie16ReadModuleInfo failed, retval = %d", retval);
            return false;
        }
        spdlog::info("Booting module %d, serial: %d, rev: %d, ADC: %d, MSPS: %d",
                module, rev, adc_bits, msps);
        auto fw = fw_map.find({rev, adc_bits, msps});
        if ( fw == fw_map.end() ){
            spdlog::error("Could not find firmware.");
            return false;
        }

        retval = interface.PixieBootModule(fw->second.FPGA_com.c_str(),
                                           fw->second.FPGA_SP.c_str(),
                                           nullptr,
                                           fw->second.DSP_code.c_str(),
                                           settings,
                                           fw->second.DSP_var.c_str(),
                                           module,
                                           0x7F);
        if ( retval < 0 ){
            spdlog::error("Pixie16BootModule failed, retval = %d", retval);
            return false;
        }
        spdlog::info("Module %d: Boot successful", module);
    }
    return true;
}

bool ExitSystem()
{
    spdlog::info("Exiting XIA system...");
    auto retval = XIAinterface::Get().PixieExitSystem(XIAinterface::Get().Get_nModules());
    if ( retval < 0 ){
        spdlog::error("Pixie16ExitSystem failed, retval = %d", retval);
        return false;
    }
    spdlog::info("XIA successfully exited");
    return true;
}

std::jthread Start_listmode(FILE *file, unsigned int *buffer)
{
    return std::jthread([&](const std::stop_token& st){
        try {
            ListModeReadout readout;
            while (!st.stop_requested())
                LMReadout_loop(file, buffer, readout, false);
            while ( LMReadout_loop(file, buffer, readout, true) ){
                // Continue running until we get false
            }
        } catch (const std::exception &err) {
            spdlog::error("Could not start readout, error '%s'", err.what());
        }
    });
}

std::jthread Start_scaler(const char *url)
{
    return std::jthread([&](const std::stop_token& st){
        try {
            ScalerReadout readout(url);
            while ( !st.stop_requested() ){
                readout.DoReadout();
                std::this_thread::sleep_for(std::chrono::milliseconds( 250 ) );
            }
        } catch ( const std::exception &err ) {
            spdlog::error(err.what());
        }
    });
}


int main()
{

    // First thing is to initialize the XIAinterface.
    try {
        XIAinterface &xia_interface = XIAinterface::Get();
    } catch (std::runtime_error &err) {
        spdlog::error(err.what());
#if !__APPLE__
        exit(EXIT_FAILURE);
#endif // !__APPLE__
    }

    auto fw_map = ReadConfigFile("firmware.txt");
    bootXIA(fw_map, "settings.set");

    // Sleep for a second
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ExitSystem();

    std::map<std::string, double> map;

    map["PIXIE16_SOMETHING"] = 0.33;
    map["PIXIE16_SOMETHING_ELSE"] = 11.2;

    std::ostringstream stream;
    {
        cereal::JSONOutputArchive archive( stream );
        archive( CEREAL_NVP(map) );
    }

    httplib::Server srv;

    srv.Get("/parameters", [&](const httplib::Request &req, httplib::Response &resp)
    {
        resp.status = 200;
        resp.set_content(stream.str(), "application/json");
    });

    srv.Get(R"(/test/?module_(\d+))", [](const httplib::Request &req, httplib::Response &resp){
        for ( auto &m : req.matches ){
            std::cout << m << std::endl;
        }
        auto number = req.matches[1];
        resp.set_content(number, "text/plain");
    });

    //srv.listen("localhost", 1234);
    std::cout << "Exiting..." << std::endl;
    return 0;
}