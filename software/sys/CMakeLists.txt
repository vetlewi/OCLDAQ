cmake_minimum_required(VERSION 3.13)
project(OCLDAQ)

add_definitions(-DPLX_LITTLE_ENDIAN -DPCI_CODE -DPLX_LINUX)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../plx
        ${CMAKE_CURRENT_SOURCE_DIR}/../sys
        ${CMAKE_CURRENT_SOURCE_DIR}/../app)

add_library(Pixie16Sys STATIC
        communication.c
        i2cm24c64.c
        pixie16sys.c
        tools.c
        pixie16sys_common.h
        pixie16sys_defs.h
        pixie16sys_export.h
        pixie16sys_globals.h)