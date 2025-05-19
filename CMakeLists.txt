cmake_minimum_required(VERSION 3.28.3)
project(RailwayPlatformOptimization)

set(SOURCES 
    src/main.cpp
    src/CSVReader.cpp
    src/MinHeap.cpp
    src/PlatformAllocator.cpp
    src/Train.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})