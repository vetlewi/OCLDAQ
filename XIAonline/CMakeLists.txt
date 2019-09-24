cmake_minimum_required(VERSION 3.14)
project(OCLDAQ)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../sirius)

add_executable(XIAonline
        main.cpp
        Sort_Funct.cpp
        Event_builder.cpp
        experimentsetup.c
        Unpacker.cpp
        XIA_CFD.c
        spectrum_rw.cpp)

target_link_libraries(XIAonline sirius)