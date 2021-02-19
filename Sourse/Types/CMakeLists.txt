# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(Types)

set (TYPES_HEADERS
    "Vec2.h"
    "Hierarchy.h"
    "Strings.h"
    "Rect.h"
    "Mem.h"
    "MathMacros.h"
    "Property.h"
    "Vec3.h"
    "FrameBuff.h"
    "LinkedList.h"
    "Ray.h"
    "Timer.h"
    "Types.h"
    "Mesh.h"
    "Stack.h"
    "Matrix.h"
    "Object.h"
    "Trig.h"
    "Parser.h"
 "SortPolicy.h" "AllocatorPolicy.h" "Color.h" )


# Source files
set (TYPES_SOURCES
    "private/Property.cpp"
    
    "private/Ray.cpp"
    "private/Timer.cpp"
    "private/Mem.cpp"
    "private/Strings.cpp"
    "private/Mesh.cpp"
    "private/Object.cpp"
    "private/Trig.cpp"
    "private/Parser.cpp"
 "private/SortPolicy.cpp" "private/AllocatorPolicy.cpp")

# Create the library
add_library(${PROJECT_NAME} ${TYPES_HEADERS} ${TYPES_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#target_include_directories(${PROJECT_NAME} PUBLIC include)

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Types>
)