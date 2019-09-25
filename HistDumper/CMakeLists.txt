cmake_minimum_required(VERSION 3.13)
project(rupdate)

#set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk)


# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package(ROOT COMPONENTS Core Imt RIO Net Hist Graf Graf3d Gpad ROOTDataFrame Tree TreePlayer Rint Postscript Matrix Physics MathCore Thread MultiProc)
include(${ROOT_USE_FILE})
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../sirius
        ${CMAKE_CURRENT_SOURCE_DIR}/../software/app
        ${CMAKE_CURRENT_SOURCE_DIR}/../software/sys
        ${CMAKE_CURRENT_SOURCE_DIR}/../software/inc
        ${CMAKE_CURRENT_SOURCE_DIR})

#set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk)
ROOT_GENERATE_DICTIONARY(histdump_dict InterfaceDumper.h LINKDEF LinkDef.h)

add_executable(HistDumper main.cpp InterfaceDumper.cpp timercpp.hpp SettingsDialog.cpp SettingsDialog.h histdump_dict.cxx)
target_link_libraries(HistDumper pthread Pixie16App Pixie16Sys Plx sirius dl ${ROOT_LIBRARIES})
