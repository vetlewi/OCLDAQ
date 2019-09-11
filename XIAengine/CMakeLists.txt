cmake_minimum_required(VERSION 3.13)
project(OCLDAQ)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-D_FILE_OFFSET_BITS=64)


set(XIAengine_src
        engine.cpp
        functions.cpp
        WriteTerminal.cpp
        XIAControl.cpp
        Functions.h
        WriteTerminal.h
        XIAControl.h ScalerWriter.cpp ScalerWriter.h FileScalerWriter.cpp FileScalerWriter.h)

find_package(Qt5 COMPONENTS core Widgets)
if ( Qt5_FOUND )
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    list(APPEND ${XIAengine_src} mainwindow.cpp mainwindow.ui mainwindow.h)
    include_directories(${QT_INCLUDES})
else()
    message(STATUS "Could not find Qt5")
endif()

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/software/sys
        ${CMAKE_CURRENT_SOURCE_DIR}/software/app
        ${CMAKE_CURRENT_SOURCE_DIR}/software/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../sirius
)

add_subdirectory(software/app)
add_subdirectory(software/sys)
add_subdirectory(software/plx)

add_executable(XIAengine ${XIAengine_src})

if ( Qt5_FOUND )
    target_link_libraries(XIAengine Pixie16App Pixie16Sys Plx sirius ${QT_LIBRARY_DIR} dl)
    target_link_options(XIAengine -ldl)
else()
    target_link_libraries(XIAengine Pixie16App Pixie16Sys Plx sirius dl)
endif()