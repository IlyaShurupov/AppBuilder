# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

# Project configuration
project(Application)


set (HEADERS
    "Application.h"
    
    "Platform/private/Win32Window.h" 
    "Platform/SysHandler.h" 
    
    "Operator/Operator.h"
    
    "Data/Seance.h"  
    
    "UI/UInterface.h"
    "UI/UInputsMap.h"
    "UI/UInputs.h" 
    
    "Object/Hierarchy.h"
    "Object/Property.h"
    "Object/Object.h"
    )


# Source files
set (SOURCES
    "Application.cpp"
    
    "Object/private/Property.cpp"
    "Object/private/Object.cpp"
    
    "Operator/private/Operator.cpp"
    
    "Data/private/Seance.cpp"
    
    "Platform/private/Win32Window.cpp"
    "Platform/private/SysHandler.cpp" 

    "UI/private/UInterface.cpp"
    "UI/private/UInputsMap.cpp" 
    )

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Types>
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Application>
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(${PROJECT_NAME} "d2d1.lib" "Types.lib")
