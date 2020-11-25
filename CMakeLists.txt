cmake_minimum_required(VERSION 3.17)
project(NESemulator)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES ./src/main.cpp src/Bus.h src/Bus.cpp src/CPU6502.cpp src/CPU6502.h src/CompactLogger.cpp src/CompactLogger.h src/tests.cpp src/tests.h src/Cartridge.cpp src/Cartridge.h src/Mapper.cpp src/Mapper.h)



add_executable(NESemulator ${SOURCE_FILES})

