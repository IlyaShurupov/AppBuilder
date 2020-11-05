# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(RayCast)

set (PROJECT_HEADERS
    "public/RayCast.h"
)

set (PROJECT_SOURCES
    "RayCast.cpp"
)


add_library(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})
