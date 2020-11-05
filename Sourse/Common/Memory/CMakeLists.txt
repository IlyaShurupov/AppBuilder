# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(Memory)

set (PROJECT_HEADERS
    "public/Memory.h"
)

set (PROJECT_SOURCES
    "Memory.cpp"
)


add_library(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})
