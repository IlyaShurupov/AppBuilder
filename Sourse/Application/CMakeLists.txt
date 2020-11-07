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

link_directories(${CMAKE_BINARY_DIR}/bin)

# Create proj
add_executable(${PROJECT_NAME} ${TYPES_HEADERS} ${TYPES_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Types/public>
              $<INSTALL_INTERFACE:/Sourse/Common/Types/public>
)

#target_link_libraries (${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Sourse/Types)
target_link_libraries(${PROJECT_NAME} Types)
#install(TARGETS ${PROJECT_NAME} DESTINATION bin)