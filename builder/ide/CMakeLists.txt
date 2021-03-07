# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

# Project configuration
project(AppBuilder)


get_filename_component(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

set(BINARY_DIR "${ROOT_DIR}/build")
set(SRS_DIR "${ROOT_DIR}/sourse")

#set(GLOBAL ROOT_DIR)

add_subdirectory("${ROOT_DIR}/sourse" "${BINARY_DIR}/sourse")
add_subdirectory("${ROOT_DIR}/tests" "${BINARY_DIR}/tests")
add_subdirectory("${ROOT_DIR}/samples/DrawApp" "${BINARY_DIR}/samples/DrawApp")

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


set(GLOBAL CMAKE_EXPORT_COMPILE_COMMANDS ON)

