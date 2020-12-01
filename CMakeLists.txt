cmake_minimum_required(VERSION 3.17)
project(NESemulator)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES ./src/main.cpp src/Bus.h src/Bus.cpp src/CPU6502.cpp src/CPU6502.h src/CompactLogger.cpp src/CompactLogger.h src/tests.cpp src/tests.h src/PPU2C02.cpp src/PPU2C02.h)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


set (SFML_DIR "${PROJECT_SOURCE_DIR}/external/SFML/lib/cmake/SFML/")
set(SFML_STATIC_LIBRARIES TRUE)


find_package (SFML 2 COMPONENTS window system REQUIRED)
find_package (OpenGL)

add_executable(NESemulator ${SOURCE_FILES})

target_link_libraries (NESemulator ${OPENGL_LIBRARIES} sfml-window sfml-system)
