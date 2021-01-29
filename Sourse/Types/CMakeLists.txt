# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(Types)

set (TYPES_HEADERS
    "public/Vec2.h"
    "public/Hierarchy.h"
    "public/Strings.h"
    "public/Rect.h"
    "public/Mem.h"
    "public/MathMacros.h"
    "public/Property.h"
    "public/Vec3.h"
    "public/FrameBuff.h"
    "public/LinkedList.h"
    "public/Ray.h"
    "public/Timer.h"
    "public/Types.h"
    "public/Mesh.h"
    "public/Stack.h"
    "public/Matrix.h"
    "public/Object.h"
    "public/Trig.h"
)


# Source files
set (TYPES_SOURCES
    "Property.cpp"
    "FrameBuff.cpp"
    "Ray.cpp"
    "Timer.cpp"
    "Mem.cpp"
    "Strings.cpp"
    "Mesh.cpp"
    "Object.cpp"
    "Trig.cpp"
)

# Create the library
add_library(${PROJECT_NAME} ${TYPES_HEADERS} ${TYPES_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#target_include_directories(${PROJECT_NAME} PUBLIC include)

#target_include_directories(${PROJECT_NAME} PUBLIC
#             $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Common/Memory/public>
#             $<INSTALL_INTERFACE:/Sourse/Common/Memory/public>
#)