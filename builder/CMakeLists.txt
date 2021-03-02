# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

# Project configuration
project(AppBuilder)


# Enable testing
enable_testing()

get_filename_component(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

set(BINARY_DIR "${ROOT_DIR}/build/bin")
set(IDE_DIR "${ROOT_DIR}/build/ide")
set(SRS_DIR "${ROOT_DIR}/sourse")

message( ${IDE_DIR})

set(CMAKE_BINARY_DIR ${IDE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}")

#set(GLOBAL ROOT_DIR)

add_subdirectory("${ROOT_DIR}/sourse" "${IDE_DIR}/sourse")
add_subdirectory("${ROOT_DIR}/tests" "${IDE_DIR}/tests")
add_subdirectory("${ROOT_DIR}/samples" "${IDE_DIR}/samples")

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


set(GLOBAL CMAKE_EXPORT_COMPILE_COMMANDS ON)
