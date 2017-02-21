cmake_minimum_required(VERSION 2.8)
project(Athena)

set(CMAKE_CXX_STANDARD 11)
set(TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(DEVICE)
else()
    message(FATAL_ERROR "select a device!!!")
endif()

include(${TOP_DIR}/devices/${DEVICE}/device.cmake)
