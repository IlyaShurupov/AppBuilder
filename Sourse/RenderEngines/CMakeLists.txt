# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

add_subdirectory("Types")
add_subdirectory("Raster")


set_target_properties(RayCast Raster
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_target_properties(RayCast Raster
   PROPERTIES
   FOLDER "RenderEngines")