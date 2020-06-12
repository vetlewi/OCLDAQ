//
// Created by Vetle Wegner Ingeberg on 12/06/2020.
//

#include "utils.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>


static std::string exec(const char* cmd) {
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

std::vector<unsigned short> utils::ReadSlotMap()
{

    auto devices = exec("lspci | grep \"PLX Technology, Inc. PCI9054 32-bit 33MHz PCI <-> IOBus Bridge (rev 0b)\"");
    std::vector<unsigned short> slotMap;

    std::cout << devices << std::endl;

    // Next we need to decode the output. We read the string, line by line.
    std::istringstream stream(devices);
    std::string line;
    while ( std::getline(stream, line) ){
        // First we need to decode the first two characters
        std::string PCIBusNumber(line.begin(), line.begin()+2);
        std::string PCIDevNumber(line.begin()+3, line.begin()+5);
        std::cout << "PCIBusNumber: " << PCIBusNumber << ", PCIDevNumber: " << PCIDevNumber << std::endl;

        // We know that slot 2 -> (3,15), 3 -> (3,14), 4 -> (3,13), 5 -> (3, 12), 6 -> (3, 11), etc.
        unsigned short dev_id;
        if ( std::stoi(PCIBusNumber) - 3 == 0 ){
            dev_id = 2 + (15 - std::stoi(PCIDevNumber, nullptr, 16));
        } else if ( std::stoi(PCIBusNumber) - 3 == 1 ){
            dev_id = 8 + (15 - std::stoi(PCIDevNumber, nullptr, 16));
        }
        slotMap.push_back(dev_id);
    }
    return slotMap;
}