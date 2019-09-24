cmake_minimum_required(VERSION 3.14)
project(OCLDAQ)

add_definitions(-DPLX_LITTLE_ENDIAN -DPCI_CODE -DPLX_LINUX)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../plx
        ${CMAKE_CURRENT_SOURCE_DIR}/../sys
        ${CMAKE_CURRENT_SOURCE_DIR}/../app)

add_library(Pixie16App STATIC
        pixie16app.c
        utilities.c
        pixie16app_common.h
        pixie16app_defs.h
        pixie16app_export.h
        pixie16app_globals.h
        xia_common.h)