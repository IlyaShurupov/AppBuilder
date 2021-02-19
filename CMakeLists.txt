# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

# Project configuration
project(Gamuncool VERSION 0.8.0 LANGUAGES CXX)

# Set default build type
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


# Enable testing
enable_testing()

add_subdirectory("Sourse")
add_subdirectory("Tests")

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(GLOBAL CMAKE_EXPORT_COMPILE_COMMANDS ON)
