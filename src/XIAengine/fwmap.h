//
// Created by Vetle Wegner Ingeberg on 10/12/2020.
//

#ifndef OCLDAQ_FWMAP_H
#define OCLDAQ_FWMAP_H

#include <map>
#include <string>

/*!
 * A class to read and store needed variables for firmware.
 */
class fwmap
{
public:
    struct xia_module_t {
        unsigned short rev;
        unsigned short bit;
        unsigned short mhz;

        bool is_valid() const {
            if (!(rev == 0xb || rev == 0xc || rev == 0xd || rev == 0xf))
                return false;
            if ( !(bit == 12 || bit == 14 || bit == 16) )
                return false;
            if ( !(mhz == 100 || mhz == 250 || mhz == 500) )
                return false;
            return true;
        }
    };

    struct fwmap_t {
        std::string ComFPGA;
        std::string SPFPGA;
        std::string DSPCode;
        std::string DSPVar;

        inline bool is_valid() const {
            if ( ComFPGA.empty() || SPFPGA.empty() ||DSPCode.empty() || DSPVar.empty() )
                return false;
            return true;
        }
    };

private:

    std::map<xia_module_t, fwmap_t> mapping;

public:

    explicit fwmap(const char *fname);

    fwmap_t GetFW(const xia_module_t &key) const;

};


#endif //OCLDAQ_FWMAP_H
