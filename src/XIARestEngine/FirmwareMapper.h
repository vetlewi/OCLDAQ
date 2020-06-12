//
// Created by Vetle Wegner Ingeberg on 12/06/2020.
//

#ifndef FIRMWAREMAPPER_H
#define FIRMWAREMAPPER_H

#include <map>
#include <unordered_map>
#include <string>

namespace spdlog {
    class logger;
}

typedef struct module_firmware_key {
    unsigned short ModRev;
    unsigned short ModADCBits;
    unsigned short ModADCMSPS;

    friend bool operator<(const module_firmware_key &lhs, const module_firmware_key &rhs);
} module_firmware_key_t;


typedef struct {
    std::string FPGA_com;
    std::string FPGA_SP;
    std::string DSP_code;
    std::string DSP_var;
} module_firmware_entry_t;

typedef std::map<module_firmware_key_t, module_firmware_entry_t> firmware_map_t;

extern firmware_map_t ReadConfigFile(const char *fname, std::shared_ptr<spdlog::logger> logger = nullptr);

#endif //OCLDAQ_FIRMWAREMAPPER_H
