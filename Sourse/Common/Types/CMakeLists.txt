# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(Types)

# Types project

set (TYPES_HEADERS
    "public/Vec2.h"
    "public/Vec3.h"
    "public/FrameBuff.h"
    "public/LinkedList.h"
    "public/Ray.h"
    "public/Timer.h"
    "public/Mesh.h"
)

# Source files
set (TYPES_SOURCES
    "Vec2.cpp"
    "Vec3.cpp"
    "FrameBuff.cpp"
    "LinkedList.cpp"
    "Ray.cpp"
    "Timer.cpp"
    "Mesh.cpp"
)

# Create the library
add_library(Types ${TYPES_HEADERS} ${TYPES_SOURCES})

target_include_directories(Types PUBLIC
              $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>
)

set_target_properties(Types
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_target_properties(Types
   PROPERTIES
   FOLDER "Common")