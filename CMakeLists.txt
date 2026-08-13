cmake_minimum_required(VERSION 3.10)
project(Styx LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable Position Independent Code for shared library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Output directories for build artifacts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # Executables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # Shared libs (.so)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # Static libs (.a)

include_directories(${CMAKE_SOURCE_DIR}/include)    # Public API headers
include_directories(${CMAKE_SOURCE_DIR})            # For util/ files

add_subdirectory(util)                              # Build utilities first (no dependencies)
add_subdirectory(src/core)                          # Build core library
add_subdirectory(src/isa)                           # Build ISA library
add_subdirectory(src/handlers)                      # Build handlers library

# Combined shared library for Python TUI
# Links all object files from static libraries into libstyx.so
add_library(styx SHARED
    $<TARGET_OBJECTS:styx-util>
    $<TARGET_OBJECTS:styx-core>
    $<TARGET_OBJECTS:styx-isa>
    $<TARGET_OBJECTS:styx-handlers>
)

# Test executable comes last (depends on libraries)
add_subdirectory(test)