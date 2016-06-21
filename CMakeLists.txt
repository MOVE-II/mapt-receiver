cmake_minimum_required(VERSION 3.2)

project(mapt-receiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES src/main.cpp src/UDPListener.cpp src/DataHandler.cpp src/Command.cpp)
add_executable(mapt-receiver ${SOURCE_FILES})

find_package(Threads)
target_link_libraries(mapt-receiver ${CMAKE_THREAD_LIBS_INIT})