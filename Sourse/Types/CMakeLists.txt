# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(Types)

set (TYPES_HEADERS
    "Geometry/Vec2.h"
    "Hierarchy.h"
    "Strings.h"
    "Geometry/Rect.h"
    "Memory/Mem.h"
    "Macros.h"
    "Property.h"
    "Geometry/Vec3.h"
    "Color/FrameBuff.h"
    "Containers/List.h"
    "Geometry/Ray.h"
    "Time/Timer.h"
    "Types.h"
    "Geometry/Mesh.h"
    "Containers/Stack.h"
    "Geometry/Matrix.h"
    "Object.h"
    "Geometry/Trig.h"
    "IO/Parser.h"
 "Sort.h" "Memory/Allocators.h" "Color/Color.h" )


# Source files
set (TYPES_SOURCES
    "private/Property.cpp"
    
    "Geometry/private/Ray.cpp"
    "Time/private/Timer.cpp"
    "Memory/private/Mem.cpp"
    "private/Strings.cpp"
    "Geometry/private/Mesh.cpp"
    "private/Object.cpp"
    "Geometry/private/Trig.cpp"
    "IO/private/Parser.cpp"
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

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Types>
)