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
    "public/Types.h"
    "public/Mesh.h"
    "public/Stack.h"
    "public/Matrix.h"
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
    "Matrix.cpp"
)

# Create the library
add_library(${PROJECT_NAME} ${TYPES_HEADERS} ${TYPES_SOURCES})

#target_include_directories(${PROJECT_NAME} PUBLIC
#             $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Common/Memory/public>
#             $<INSTALL_INTERFACE:/Sourse/Common/Memory/public>
#)