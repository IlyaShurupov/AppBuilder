# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(RayCast)

set (PROJECT_HEADERS
    "RayCast.h"
)

set (PROJECT_SOURCES
    "private/RayCast.cpp"
)


add_library(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Types>
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/RenderEngines/RayCast>
)

target_link_libraries(${PROJECT_NAME} "Types.lib")