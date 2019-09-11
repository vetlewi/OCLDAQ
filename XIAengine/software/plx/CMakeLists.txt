cmake_minimum_required(VERSION 3.13)
project(Plx)

add_definitions(-DPLX_LITTLE_ENDIAN -DPCI_CODE -DPLX_LINUX)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)

add_library(Plx STATIC
        Aardvark.c
        Eep_8000.c
        PlxApi.c
        PlxApiI2cAa.c
        Aardvark.h
        Eep_8000.h
        PlxApiDebug.h
        PlxApiI2cAa.h)