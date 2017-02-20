cmake_minimum_required(VERSION 3.6)
project(AthenaTest)
include_directories(include)

add_executable(queue_test queue_test.cpp)
target_link_libraries(queue_test utils)

add_executable(decoder_test decoder_test.cpp)
target_link_libraries(decoder_test utils)

add_executable(thread_test thread_test.cpp)
target_link_libraries(thread_test utils)

add_executable(config_test Car.cpp)
target_link_libraries(config_test utils)


