# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(Types)

set (TYPES_HEADERS
    "Geometry/Vec2.h"
    "Geometry/Rect.h"
    "Geometry/Vec3.h"
    "Geometry/Ray.h"
    "Geometry/Mesh.h"
    "Geometry/Matrix.h"
    "Geometry/Trig.h"

    "Containers/List.h"
    "Containers/Stack.h"
    "Containers/Sort.h" 
    "Containers/Array.h" 
    
    "Memory/Mem.h"
    "Memory/Allocators.h" 
    
    "BitMap/BitMap.h"
    "BitMap/Color.h"  
    
    "Time/Timer.h"
    
    "Strings/Strings.h"
    "Macros.h"
    "IO/Parser.h"
    "Types.h"
    )


# Source files
set (TYPES_SOURCES
    "Geometry/private/Ray.cpp"
    "Geometry/private/Mesh.cpp"
    "Geometry/private/Trig.cpp"
    
    "Time/private/Timer.cpp"
    "Memory/private/Mem.cpp"
    "IO/private/Parser.cpp" 
    "Strings/private/Strings.cpp"
    )

# Create the library
add_library(${PROJECT_NAME} ${TYPES_HEADERS} ${TYPES_SOURCES})

#target_include_directories(${PROJECT_NAME} PUBLIC include)

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${SRS_DIR}/Types>
)