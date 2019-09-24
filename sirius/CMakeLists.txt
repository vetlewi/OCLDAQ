cmake_minimum_required(VERSION 3.14)
project(OCLDAQ)

add_library(sirius
        engine_shm.cpp
        net_control.cpp
        run_command.cpp
        sort_spectra.cpp
        utilities.cpp
        buffer_defs.h
        debug.h
        engine_shm.h
        ipc_defs.h
        net_control.h
        offmem_defs.h
        offspec_defs.h
        run_command.h
        sort_spectra.h
        spec_defs.h
        utilities.h)