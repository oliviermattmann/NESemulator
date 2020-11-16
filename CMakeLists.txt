cmake_minimum_required(VERSION 3.17)
project(NESemulator)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES ./src/main.cpp src/Bus.h src/Bus.cpp src/CPU6502.cpp src/CPU6502.h src/CompactLogger.cpp src/CompactLogger.h)



add_executable(NESemulator ${SOURCE_FILES})

