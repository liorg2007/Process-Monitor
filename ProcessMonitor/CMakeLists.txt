cmake_minimum_required(VERSION 3.25)
project(ProcessMonitor)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(ProcessMonitor main.cpp
        ProcessRetriever.h
        ProcessRetriever.cpp
        DataDisplay.cpp
        DataDisplay.h)

target_link_libraries(ProcessMonitor ${CURSES_LIBRARIES})
