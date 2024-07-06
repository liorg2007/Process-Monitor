cmake_minimum_required(VERSION 3.28)
project(ProcessMonitor)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProcessMonitor main.cpp
        ProcessRetriever.h
        ProcessRetriever.cpp)
