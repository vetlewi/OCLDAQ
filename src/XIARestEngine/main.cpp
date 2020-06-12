//
// Created by Vetle Wegner Ingeberg on 12/06/2020.
//

#include "XIAinterface.h"
#include "FirmwareMapper.h"
#include "utils.h"

#include <iostream>

int main()
{

    auto slots = utils::ReadSlotMap();

    for ( auto &r : slots ){
        std::cout << r << std::endl;
    }

    return 0;
}