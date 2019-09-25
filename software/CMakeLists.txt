cmake_minimum_required(VERSION 3.14)
project(OCLDAQ)


add_subdirectory(app)
add_subdirectory(sys)
add_subdirectory(plx)

add_executable(XIA_dummy dummy.cpp)
target_link_libraries(XIA_dummy Pixie16App Pixie16Sys Plx sirius dl)