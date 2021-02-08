# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(PhysEngene)

set (PROJECT_HEADERS
    "PhysEngene.h"
)

set (PROJECT_SOURCES
    "private/PhysEngene.cpp"
)


add_library(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})
