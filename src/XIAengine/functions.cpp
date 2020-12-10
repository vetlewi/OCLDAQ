#include "functions.h"

#include "pixie16app_export.h"
#include "pixie16app_defs.h"

#include <INIReader.h>

#include <array>
#include <string>
#include <sstream>
#include <map>

#if CMAKE_TEST
#include "doctest.h"
#endif // CMAKE_TEST

typedef struct {
    int PCIBusNum;
    int PCIDevNum;
} pci_addr_t;

inline bool operator<(const pci_addr_t &lhs, const pci_addr_t &rhs){
    if ( lhs.PCIBusNum < rhs.PCIBusNum )
        return true;
    if ( lhs.PCIBusNum == rhs.PCIBusNum ) {
        if (lhs.PCIDevNum < rhs.PCIDevNum )
            return true;
        return false;
    }
    return false;
}

int AdjustBaselineOffset(unsigned short modNo)
{
    // First we "back up" all the baseline cut values for the module.

    double tmp;
    unsigned int baselineCut[PRESET_MAX_MODULES][NUMBER_OF_CHANNELS];

    for (int module = 0 ; module < modNo ; ++module){
        for (int channel = 0 ; channel < NUMBER_OF_CHANNELS ; ++channel){
            Pixie16ReadSglChanPar(const_cast<char *>("BLCUT"), &tmp, module, channel);
            baselineCut[module][channel] = tmp;
        }
    }

    // Find new DC baseline offset.
    int retval = Pixie16AdjustOffsets(modNo);

    // Reset the baseline cut values.
    for (int module = 0 ; module < modNo ; ++module){
        for (int channel = 0 ; channel < NUMBER_OF_CHANNELS ; ++channel){
            Pixie16WriteSglChanPar(const_cast<char *>("BLCUT"), baselineCut[module][channel], module, channel);
        }
    }

    return retval;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}



std::map<pci_addr_t, unsigned short> GetMapping(const char *ini_file = "pxisys.ini")
{
    INIReader reader(ini_file);
    std::string slot_name;

    if ( reader.ParseError() != 0 ){
        std::string errmsg = "Could not load '" + std::string(ini_file) + "'";
        throw std::runtime_error(errmsg);
    }
    std::map<pci_addr_t, unsigned short> map;
    for ( unsigned short slot = 2 ; slot <= 14 ; ++slot ){
        slot_name = "Slot" + std::to_string(slot);
        int busNum = reader.GetInteger(slot_name, "PCIBusNumber", -1);
        int devNum = reader.GetInteger(slot_name, "PCIDeviceNumber", -1);
        if ( busNum < 0 || devNum < 0 ){
            std::string errmsg = "Could read Bus/Dev number for section '" + std::string(slot_name) + "'";
            throw std::runtime_error(errmsg);
        }
        map[{busNum, devNum}] = slot;
    }
    return map;
}

#if CMAKE_TEST
#ifndef INI_PATH
#define INI_PATH
#endif // INI_PATH

TEST_CASE("Testing functions in functions.h"){
    INFO("Trying with path to INI file " << INI_PATH);
    REQUIRE_NOTHROW(GetMapping(INI_PATH));
}
#endif // CMAKE_TEST

std::vector<unsigned short> ReadSlotMap(const char *ini_file)
{
    auto map = GetMapping(ini_file);
    auto devices = exec("lspci | grep \"PLX Technology, Inc. PCI9054 32-bit 33MHz PCI <-> IOBus Bridge (rev 0b)\"");
    std::vector<unsigned short> slotMap;

    // Next we need to decode the output. We read the string, line by line.
    std::istringstream stream(devices);
    std::string line;
    while ( std::getline(stream, line) ){
        // First we need to decode the first two characters
        std::string PCIBusNumber(line.begin(), line.begin()+2);
        std::string PCIDevNumber(line.begin()+3, line.begin()+5);

        int busNum = std::stoi(PCIBusNumber, nullptr, 16);
        int devNum = std::stoi(PCIDevNumber, nullptr, 16);


        if ( map.find({busNum, devNum}) == map.end() ){
            std::string errmsg = "Could not find PCI bus " + std::to_string(busNum);
            errmsg += ", dev " + std::to_string(devNum);
            throw std::runtime_error(errmsg);
        }

        slotMap.push_back(map[{busNum, devNum}]);
    }
    std::sort(std::begin(slotMap), std::end(slotMap));
    return slotMap;
}