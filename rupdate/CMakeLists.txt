cmake_minimum_required(VERSION 3.13)
project(OCLDAQ)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})


find_package(ROOT CONFIG REQUIRED)
include(${ROOT_USE_FILE})
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../sirius
        ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY(rupdate_dict rupdate.h io_root.h LINKDEF linkdef.h)

add_executable(rupdate rupdate.cpp io_root.cpp rupdate.h io_root.h rupdate_dict.cxx)
target_link_libraries(rupdate sirius ROOT::Core Gui)