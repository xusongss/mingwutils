cmake_minimum_required(VERSION 3.6)
project(Athena)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmakes)
set(TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include(Env)

if(DEVICE)
else()
    message(FATAL_ERROR "select a device!!!")
endif()

include(${TOP_DIR}/devices/${DEVICE}/device.cmake)
