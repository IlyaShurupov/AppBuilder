# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

project(Raster)

set (PROJECT_HEADERS
    "public/Raster.h"
)

set (PROJECT_SOURCES
    "Raster.cpp"
)


add_library(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})
