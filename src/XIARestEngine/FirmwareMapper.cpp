//
// Created by Vetle Wegner Ingeberg on 12/06/2020.
//

#include "FirmwareMapper.h"

#include <fstream>

#include <spdlog/spdlog.h>

bool operator<(const module_firmware_key &lhs, const module_firmware_key &rhs)
{
    return (lhs.ModRev < rhs.ModRev) ||
           (lhs.ModRev == rhs.ModRev && lhs.ModADCBits < rhs.ModADCBits) ||
           (lhs.ModRev == rhs.ModRev && lhs.ModADCBits == rhs.ModADCBits && lhs.ModADCMSPS < rhs.ModADCMSPS);
}

bool next_line(std::istream &in, std::string &line)
{
    line = "";

    std::string tmp;
    while ( std::getline(in, tmp) ){
        size_t ls = tmp.size();
        if ( ls == 0 ){
            break;
        } else if ( tmp[ls-1] != '\\' ){
            line += tmp;
            break;
        } else {
            line += tmp.substr(0, ls-1);
        }
    }
    return in || !line.empty();
}

std::string strip(const std::string& s)
{
    std::string::size_type start = s.find_first_not_of(" \t\r\n");
    if( start==std::string::npos )
        start = 0;

    std::string::size_type stop = s.find_last_not_of(" \t\r\n");
    if( stop==std::string::npos )
        stop = s.size()-1;

    return s.substr(start, stop+1-start);
}

std::map<std::string, std::string> SimpleReadFile(const char *fname, std::shared_ptr<spdlog::logger> logger)
{
    std::ifstream input(fname);
    std::string line;

    std::map<std::string, std::string> fw;
    if ( logger )
        logger->info("Reading firmware configuration from file %s", fname);

    if ( !input.is_open() ){
        if ( logger )
            logger->error("Error: Couldn't read firmware config. file\n");
        return std::map<std::string, std::string>();
    }

    while ( next_line(input, line) ){
        if ( line.empty() || line[0] == '#' )
            continue; // Ignore empty lines or comments.

        // Search for "=" sign on the line.
        std::string::size_type pos_eq = line.find("=");

        // If not found, write a warning and continue to next line.
        if ( pos_eq == std::string::npos ){
            if ( logger )
                logger->error("Could not understand line '%s', continuing...", line.c_str());
            continue;
        }

        std::string key = strip(line.substr(0, pos_eq));
        std::string val = strip(line.substr(pos_eq+1));

        // If the key have already been entered.
        if ( fw.find(key) != fw.end() ){
            if ( logger )
                logger->warn("Multiple definitions of '%s', ignoring", key.c_str());
            continue;
        }

        fw[key] = val;
    }
    return fw;
}

firmware_map_t ReadConfigFile(const char *fname, std::shared_ptr<spdlog::logger> logger)
{
    auto fw = SimpleReadFile(fname, logger);
    firmware_map_t map;

    // Rev. 11, 12, 13 all uses the identifier *_RevBCD
    unsigned short revs[] = {11, 12, 13, 15};
    unsigned short adc_bit[] = {12, 14, 16};
    unsigned short adc_msps[] = {100, 250, 500};

    // First we generate the keys for rev. 11, 12 and 13 as they all use the same key (at least for now)
    for ( auto rev : revs ){
        if ( rev == 15 ) // We skip the rev 15
            break;

        for ( auto bit : adc_bit ){
            for ( auto msps : adc_msps ){
                module_firmware_key_t key = {rev, bit, msps};

                // Check if all the required keys are present
                if ( (fw.find("comFPGAConfigFile_RevBCD") == fw.end())  ||
                     (fw.find("SPFPGAConfigFile_RevBCD") == fw.end())   ||
                     (fw.find("DSPCodeFile_RevBCD") == fw.end())        ||
                     (fw.find("DSPVarFile_RevBCD") == fw.end())           )
                    continue; // We don't set unless present

                map[key] = {fw.find("comFPGAConfigFile_RevBCD")->second,
                            fw.find("SPFPGAConfigFile_RevBCD")->second,
                            fw.find("DSPCodeFile_RevBCD")->second,
                            fw.find("DSPVarFile_RevBCD")->second};
            }
        }

    }

    unsigned short rev = 15;
    for ( auto bit : adc_bit ){
        for ( auto msps : adc_msps ){
            module_firmware_key_t key = {rev, bit, msps};

            std::string ident = "_" + std::to_string(msps) + "MHz_" + std::to_string(bit) + "Bit";

            // Check if all the required keys are present
            if ( (fw.find("comFPGAConfigFile_RevF"+ident) == fw.end())  ||
                 (fw.find("SPFPGAConfigFile_RevF"+ident) == fw.end())   ||
                 (fw.find("DSPCodeFile_RevF"+ident) == fw.end())        ||
                 (fw.find("DSPVarFile_RevF"+ident) == fw.end())           )
                continue; // We don't set unless present

            map[key] = {fw.find("comFPGAConfigFile_RevF"+ident)->second,
                        fw.find("SPFPGAConfigFile_RevF"+ident)->second,
                        fw.find("DSPCodeFile_RevF"+ident)->second,
                        fw.find("DSPVarFile_RevF"+ident)->second};
        }
    }
    return map;
}