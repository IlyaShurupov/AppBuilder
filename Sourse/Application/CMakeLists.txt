# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

# Project configuration
project(Application)


set (TYPES_HEADERS
    "Logic/public/Logic.h"
    "Window/public/Window.h"
)

# Source files
set (TYPES_SOURCES
    "Logic/Logic.cpp"
    "Window/Window.cpp"
    "Entry.cpp"
)

# Create the library
add_executable(${PROJECT_NAME} ${TYPES_HEADERS} ${TYPES_SOURCES})
