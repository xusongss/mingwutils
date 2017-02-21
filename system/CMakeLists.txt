cmake_minimum_required(VERSION 2.8)
project(system)
include (CheckIncludeFiles)
set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_definitions(-DANDROID_SMP=1)
add_definitions(-DHAVE_PTHREADS)

CHECK_INCLUDE_FILES("pthread.h" HAVE_PTHREADS)
CHECK_INCLUDE_FILES("sys/uio.h" HAVE_SYS_UIO_H)
CHECK_INCLUDE_FILES("endian.h" HAVE_ENDIAN_H)
if(HAVE_PTHREADS)
    add_definitions(-DHAVE_PTHREADS)
    message(STATUS "------have pthread.h")
endif()
if(HAVE_SYS_UIO_H)
    add_definitions(-DHAVE_SYS_UIO_H)
    message(STATUS "------have sys/uio.h")
endif()
if(HAVE_ENDIAN_H)
    add_definitions(-DHAVE_ENDIAN_H)
    message(STATUS "------have endian.h")
endif()

set(SOURCE_FILES
        libutils/Debug.cpp
        libutils/atomic.cpp
        libutils/Timers.cpp
        libutils/Log.cpp
        libutils/TextOutput.cpp
        libutils/RefBase.cpp
        libutils/Threads.cpp
        libutils/Mutex.cpp
        libutils/Condition.cpp
        libutils/RWLock.cpp
        libutils/VectorImpl.cpp
        libutils/SharedBuffer.cpp
        libutils/Queue.cpp
        libutils/AAtomizer.cpp
        libutils/AString.cpp
        libutils/ABitReader.cpp
        libutils/ABuffer.cpp
        libutils/ACrc16CCIT.cpp
        libutils/AHandler.cpp
        libutils/AHierarchicalStateMachine.cpp
        libutils/ALooper.cpp
        libutils/ALooperRoster.cpp
        libutils/AMessage.cpp
        libutils/base64.cpp
        libutils/hexdump.cpp
        libutils/Context.cpp

        libavutils/PixDesc.c
        libavutils/Raw.c
        libavutils/Utils.c

        libdecoder/Decoder.cpp



        )

add_library(utils  SHARED ${SOURCE_FILES})
target_link_libraries(utils pthread)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
