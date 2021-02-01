//
// Created by Vetle Wegner Ingeberg on 10/12/2020.
//

#include "fwmap.h"

#include <INIReader.h>
#include <spdlog/spdlog.h>

bool operator<(const fwmap::xia_module_t &lhs, const fwmap::xia_module_t &rhs){
    if ( lhs.rev < rhs.rev )
        return true;
    if ( lhs.rev == rhs.rev ){
        if ( lhs.bit < rhs.bit )
            return true;
        if ( lhs.bit == rhs.bit ) {
            if (lhs.mhz < rhs.mhz)
                return true;
            return false;
        }
        return false;
    }
    return false;
}

fwmap::fwmap(const char *fname)
{
    INIReader reader(fname);
    if ( reader.ParseError() != 0 ){
        std::string errmsg = "Error parsing file '" + std::string(fname) + "'";
        throw std::runtime_error(errmsg);
    }

    const auto& sections = reader.Sections();
    for ( auto &section : sections ){
        xia_module_t key;
        fwmap_t value;
        key.rev = reader.GetInteger(section, "rev", 0);
        key.bit = reader.GetInteger(section, "bit", 0);
        key.mhz = reader.GetInteger(section, "mhz", 0);
        value.ComFPGA = reader.Get(section, "comFPGA", "");
        value.SPFPGA = reader.Get(section, "SPFPGA", "");
        value.DSPCode = reader.Get(section, "DSPCode", "");
        value.DSPVar = reader.Get(section, "DSPCode", "");

        if ( !key.is_valid() || !value.is_valid() ){
            spdlog::error("Section {} in file {} is invalid, skipping...", section, fname);
            continue;
        }
        mapping.emplace(key, value);
    }
}

fwmap::fwmap_t fwmap::GetFW(const xia_module_t &key) const {
    if ( mapping.find(key) == mapping.end() ) {
        spdlog::error("Could not find firmware for module with rev={}, bit={}, mhz={}", key.rev, key.bit, key.mhz);
        throw std::runtime_error("Key error");
    }
    return mapping.at(key);
}