cmake_minimum_required(VERSION 3.13)
project(OCLDAQ VERSION 1.0.0 LANGUAGES CXX C)

#Make sure that custom modules are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

##############################################
# Declare options

option(ENABLE_MASTER "Build target master" ON)
option(ENABLE_RUPDATE "Build target rupdate" ON)
option(ENABLE_XIAENGINE "Build target XIAengine" ON)
option(ENABLE_XIAONLINE "Build target XIAonline" ON)
option(ENABLE_HISTDUMPER "Build target HistDumper" ON)


option(ENABLE_DOC "Generates the documentation target" OFF)
option(ENABLE_COVERAGE "Generates the coverage build" OFF)
option(ENABLE_TESTING "Turns on testing" OFF)

#set(CMAKE_VERBOSE_MAKEFILE 1)

##############################################
# Declare dependencies

if ( ENABLE_MASTER )

    find_package(X11)
    if (X11_FOUND)
        message(STATUS "Found X11")
    else()
        message(ERROR "Could not find X11, MASTER is disabled")
        set(ENABLE_MASTER OFF)
    endif()

    find_package(Motif)
    if (MOTIF_FOUND)
        message(STATUS "Found libMotif")
    else()
        message(ERROR "Could not find libMotif, MASTER is disabled")
        set(ENABLE_MASTER OFF)
    endif()
endif()

if ( ENABLE_RUPDATE OR ENABLE_HISTDUMPER )
    list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

    find_package(ROOT CONFIG)

    if ( ROOT_FOUND )
        message(STATUS "Found ROOT")
        include("${ROOT_DIR}/modules/RootNewMacros.cmake")
    else()
        message(ERROR "Could not find ROOT, rupdate and histdumper are disabled")
        set(ENABLE_RUPDATE OFF)
        set(ENABLE_HISTDUMPER OFF)
    endif()

endif()

if ( ENABLE_XIAENGINE )

    find_package(Qt5 COMPONENTS Widgets)

    if ( Qt5_FOUND )
        message(STATUS "Found Qt5")
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)
        set(CMAKE_AUTOUIC ON)
    else()
        message(STATUS "Could not find Qt5, building XIAengine without GUI")
    endif()

endif()

##############################################
# Create target and set properties

add_library(sirius STATIC
        src/sirius/engine_shm.cpp
        src/sirius/net_control.cpp
        src/sirius/run_command.cpp
        src/sirius/sort_spectra.cpp
        src/sirius/utilities.cpp
)

add_library(${PROJECT_NAME}::sirius ALIAS sirius)

target_include_directories(sirius
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sirius
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sirius
)

set_target_properties(sirius PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

add_library(plx STATIC
        src/plx/Aardvark.c
        src/plx/Eep_8000.c
        src/plx/PlxApi.c
        src/plx/PlxApiI2cAa.c)

add_library(${PROJECT_NAME}::plx ALIAS plx)

target_link_libraries(plx PUBLIC ${CMAKE_DL_LIBS})

target_include_directories(plx
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/plx
        ${CMAKE_CURRENT_SOURCE_DIR}/include/plx
)

target_compile_definitions(plx PUBLIC
        -DPLX_LITTLE_ENDIAN
        -DPCI_CODE
        -DPLX_LINUX)

add_library(xia STATIC
        src/xia/communication.c
        src/xia/i2cm24c64.c
        src/xia/pixie16app.c
        src/xia/pixie16sys.c
        src/xia/tools.c
        src/xia/utilities.c
)

add_library(${PROJECT_NAME}::xia ALIAS xia)

target_include_directories(xia
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/xia
        ${CMAKE_CURRENT_SOURCE_DIR}/include/xia
)

target_compile_definitions(xia PUBLIC
        -DPLX_LITTLE_ENDIAN
        -DPCI_CODE
        -DPLX_LINUX)

target_link_libraries(xia PUBLIC ${PROJECT_NAME}::plx)

if ( ENABLE_MASTER )
    add_executable(master
        src/master/acq_clear.cpp
        src/master/acq_dump.cpp
        src/master/acq_gui.cpp
        src/master/acq_init.cpp
        src/master/acq_reload.cpp
        src/master/acq_start.cpp
        src/master/acq_status.cpp
        src/master/acq_stop.cpp
        src/master/acq_storage.cpp
        src/master/io_xtapp.cpp
        src/master/m_engine.cpp
        src/master/m_sort.cpp
        src/master/net_client.cpp)

    target_include_directories(master
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/master
            ${MOTIF_INCLUDE_DIR}
    )

    target_link_libraries(master
            ${PROJECT_NAME}::sirius
            X11::X11
            X11::Xt
            ${MOTIF_LIBRARIES})
endif()

if ( ENABLE_RUPDATE )

    root_generate_dictionary(G__rupdate
            ${CMAKE_CURRENT_SOURCE_DIR}/src/rupdate/io_root.h
            ${CMAKE_CURRENT_SOURCE_DIR}/src/rupdate/rupdate.h
            OPTIONS -I${CMAKE_CURRENT_SOURCE_DIR}/include
            LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/rupdate/linkdef.h
    )

    add_executable(rupdate
            src/rupdate/io_root.cpp
            src/rupdate/rupdate.cpp
            G__rupdate
    )

    target_include_directories(rupdate
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/rupdate
    )

    set_target_properties(rupdate PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO)

    target_link_libraries(rupdate
            ${PROJECT_NAME}::sirius
            ROOT::Core
            ROOT::Gpad
            ROOT::Hist
            ROOT::Gui)

endif()

if ( ENABLE_XIAENGINE )

    add_executable(XIAengine
            src/XIAengine/engine.cpp
            #src/XIAengine/FileScalerWriter.cpp
            src/XIAengine/functions.cpp
            #src/XIAengine/ScalerWriter.cpp
            src/XIAengine/WriteTerminal.cpp
            src/XIAengine/XIAControl.cpp
    )

    if ( Qt5_FOUND )
        target_sources(XIAengine PRIVATE
                src/XIAengine/mainwindow.cpp
                src/XIAengine/mainwindow.ui)
        target_compile_definitions(XIAengine PRIVATE BUILD_GUI=1)
    endif()

    target_include_directories(XIAengine
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/XIAengine
            /opt/xia/current/app
            /Users/vetlewi/Desktop/XIA_stuff/Pixie16_software_linux/software/app
            /Users/vetlewi/Desktop/XIA_stuff/Pixie16_software_linux/software/sys
            /Users/vetlewi/Desktop/XIA_stuff/Pixie16_software_linux/software/inc
    )

    target_compile_definitions(XIAengine PRIVATE -D_FILE_OFFSET_BITS=64)

    set_target_properties(XIAengine PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO)

    target_link_options(XIAengine PRIVATE
            -L/opt/xia/current -L/opt/plx/current/PlxSdk/PlxApi/Library)

    target_link_libraries(XIAengine
            ${PROJECT_NAME}::sirius
            #${PROJECT_NAME}::xia
            -lPixie16App -lPixie16Sys -lPlxApi -ldl -lrt
            Qt5::Widgets)

    add_executable(XIAengineV2
            src/XIAengineV2/XIAinterface.cpp)

    set_target_properties(XIAengineV2 PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO)

    target_link_libraries(XIAengineV2 ${PROJECT_NAME}::xia)

endif()

if ( ENABLE_HISTDUMPER )

    root_generate_dictionary(G__HistDumper
            ${CMAKE_CURRENT_SOURCE_DIR}/src/HistDumper/InterfaceDumper.h
            ${CMAKE_CURRENT_SOURCE_DIR}/src/HistDumper/SettingsDialog.h
            OPTIONS -I${CMAKE_CURRENT_SOURCE_DIR}/include
            LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/HistDumper/LinkDef.h
    )

    add_executable(HistDumper
            src/HistDumper/InterfaceDumper.cpp
            src/HistDumper/main.cpp
            src/HistDumper/SettingsDialog.cpp
            src/HistDumper/timercpp.cpp
            G__HistDumper
    )

    target_include_directories(HistDumper
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/HistDumper
    )

    set_target_properties(HistDumper PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO)

    target_link_libraries(HistDumper
            ${PROJECT_NAME}::sirius
            ${PROJECT_NAME}::xia
            ROOT::Core
            ROOT::Gpad
            ROOT::Hist
            ROOT::Gui)
endif()

if ( ENABLE_XIAONLINE )

    add_executable(XIAonline
            src/XIAonline/main.cpp
            src/XIAonline/Sort_Funct.cpp
            src/XIAonline/Event_builder.cpp
            src/XIAonline/experimentsetup.c
            src/XIAonline/Unpacker.cpp
            src/XIAonline/XIA_CFD.c
            src/XIAonline/spectrum_rw.cpp)

    target_link_libraries(XIAonline
            ${PROJECT_NAME}::sirius)

    set_target_properties(XIAonline PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO)

endif()

##############################################
# Installation instructions


include(GNUInstallDirs)

install(TARGETS master rupdate XIAengine HistDumper XIAonline
    EXPORT ${PROJECT_NAME}-targets
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)