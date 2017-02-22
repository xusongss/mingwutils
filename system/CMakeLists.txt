project(system)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_FILES
        libutils/Debug.cpp
        libutils/atomic.cpp
        libutils/Timers.cpp
        libutils/Log.cpp
        libutils/TextOutput.cpp
        libutils/RefBase.cpp
        libutils/Threads.cpp
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
