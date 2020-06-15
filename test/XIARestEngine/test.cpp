//
// Created by Vetle Wegner Ingeberg on 12/06/2020.
//

void TestFWMapper()
{
    firmware_map_t map = ReadConfigFile("../test/firmware.txt");

    for ( auto &entry : map ){
        std::cout << "Rev: " << entry.first.ModRev;
        std::cout << ", ADC: " << entry.first.ModADCBits;
        std::cout << ", MSPS: " << entry.first.ModADCMSPS;
        std::cout << "\n";
        std::cout << "\tcomFPGA: " << entry.second.FPGA_com << "\n";
        std::cout << "\tSPFPGA: " << entry.second.FPGA_SP << "\n";
        std::cout << "\tDSPcode: " << entry.second.DSP_code << "\n";
        std::cout << "\tDSPvar: " << entry.second.DSP_var << "\n";
    }
}

void TestFindSlotMap()
{
    auto slots = utils::ReadSlotMap();

    for ( auto &r : slots ){
        std::cout << r << std::endl;
    }
}