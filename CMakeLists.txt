cmake_minimum_required(VERSION 3.17)
project(NESemulator)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES ./src/main.cpp src/Bus.h src/Bus.cpp src/CPU6502.cpp src/CPU6502.h util/CompactLogger.cpp util/CompactLogger.h util/tests.cpp util/tests.h src/Cartridge.cpp src/Cartridge.h src/Mapper.cpp src/Mapper.h src/PPU2C02.cpp src/PPU2C02.h util/NesTestParser.cpp util/NesTestParser.h src/Screen.cpp src/Screen.h src/NESemulator.cpp src/NESemulator.h)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


set (SFML_DIR "${PROJECT_SOURCE_DIR}/external/SFML/lib/cmake/SFML/")
set(SFML_STATIC_LIBRARIES TRUE)


find_package (SFML 2 COMPONENTS window system REQUIRED)
find_package (OpenGL)

add_executable(NESemulator ${SOURCE_FILES})

target_link_libraries (NESemulator ${OPENGL_LIBRARIES} sfml-window sfml-system sfml-graphics)
