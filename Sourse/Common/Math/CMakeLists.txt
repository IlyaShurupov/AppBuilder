# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(Math)

set (PROJECT_HEADERS
    "public/Math.h"
)

set (PROJECT_SOURCES
    "Math.cpp"
)


add_library(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})
