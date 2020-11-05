# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(Types)

set (TYPES_HEADERS
    "public/Vec2.h"
    "public/Vec3.h"
    "public/FrameBuff.h"
    "public/LinkedList.h"
    "public/Ray.h"
    "public/Timer.h"
    "public/Mesh.h"
    "public/Stack.h"
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
    "Stack.cpp"
)

# Create the library
add_library(${PROJECT_NAME} ${TYPES_HEADERS} ${TYPES_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>
)