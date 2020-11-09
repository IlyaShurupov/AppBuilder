# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

# Project configuration
project(Application)


set (HEADERS
    "public/Logic.h"
    "public/Operator.h"
    "public/Context.h"
)

# Source files
set (SOURCES
    "Logic.cpp"
    "Operator.cpp"
    "Context.cpp"
    "Entry.cpp"
)

link_directories(${CMAKE_BINARY_DIR}/bin)

# Create proj
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Types/public>
              $<INSTALL_INTERFACE:/Sourse/Types/public>
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/RenderEngines/RayCast/public>
              $<INSTALL_INTERFACE:/Sourse/RenderEngines/RayCast/public>
)
target_link_libraries(${PROJECT_NAME} Types RayCast)

#target_link_libraries (${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Sourse/Types)

#install(TARGETS ${PROJECT_NAME} DESTINATION bin)