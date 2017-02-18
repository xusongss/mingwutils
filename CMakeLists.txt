cmake_minimum_required(VERSION 3.6)
project(Athena)

set(CMAKE_CXX_STANDARD 11)
include_directories(include)
add_definitions(-DANDROID_SMP=1)
add_definitions(-DHAVE_PTHREADS)
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
        main.cpp)
add_library(utils  ${SOURCE_FILES})
add_executable(Athena ${SOURCE_FILES})