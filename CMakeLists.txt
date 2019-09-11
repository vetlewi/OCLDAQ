cmake_minimum_required(VERSION 3.14)
project(OCLDAQ)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(sirius)
add_subdirectory(master)
add_subdirectory(rupdate)
add_subdirectory(XIAengine)
add_subdirectory(XIAonline)

add_executable(OCLDAQ main.cpp)
add_dependencies(OCLDAQ sirius acq_master rupdate XIAengine XIAonline)