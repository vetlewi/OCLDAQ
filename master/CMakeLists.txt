cmake_minimum_required(VERSION 3.14)
project(acq_master)


find_package(X11)
if (X11_FOUND)
    message(STATUS "Found X11")
else()
    message(FATAL_ERROR "Could not find X11")
endif()


find_package(Motif)
if (MOTIF_FOUND)
    message(STATUS "Found libMotif")
else()
    message(FATAL_ERROR "Could not find libMotif")
endif()

include_directories(
        ${X11_INCLUDE_DIR}
        ${X11_Xt_INCLUDE_PATH}
        ${MOTIF_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../sirius
)

add_executable(acq_master
        acq_clear.cpp
        acq_dump.cpp
        acq_gui.cpp
        acq_init.cpp
        acq_reload.cpp
        acq_start.cpp
        acq_status.cpp
        acq_stop.cpp
        acq_storage.cpp
        io_xtapp.cpp
        m_engine.cpp
        m_sort.cpp
        net_client.cpp
        acq_gui.h
        acq_log.h
        io_xtapp.h
        m_control.h
        m_engine.h
        m_sort.h
        net_client.h
        )

target_link_libraries(acq_master ${X11_LIBRARIES} ${MOTIF_LIBRARIES} ${X11_Xt_LIB} sirius)