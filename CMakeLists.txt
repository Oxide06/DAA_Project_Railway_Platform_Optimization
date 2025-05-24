cmake_minimum_required(VERSION 3.28.3)
project(RailwayPlatformOptimization)

find_package(SFML COMPONENTS graphics window system REQUIRED)

set(SOURCES 
    src/main.cpp
    src/CSVReader.cpp
    src/MinHeap.cpp
    src/PlatformAllocator.cpp
    src/Train.cpp
    src/Simulation.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)