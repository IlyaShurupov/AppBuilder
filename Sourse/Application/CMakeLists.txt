# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

# Project configuration
project(Gamuncool)


set (HEADERS
    "public/Win32API.h"
    "public/Print.h"
    "public/Operator.h"
    "public/Seance.h"
    "UI/public/Window.h"
    "UI/public/UITemplates.h"
    "UI/public/UInterface.h"
    "UI/public/UInputsMap.h"
    "UI/public/UInputs.h")


# Source files
set (SOURCES
    "UI/Window.cpp"
    "Operator.cpp"
    "Seance.cpp"
    "Main.cpp"
    "UI/UInterface.cpp"
    "Win32API.cpp"
    "UI/UITemplates.cpp"
    "UI/UInputsMap.cpp"
    "Log.cpp")



# Create proj
link_directories(${CMAKE_BINARY_DIR}/bin )
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Types/public>
              $<INSTALL_INTERFACE:/Sourse/Types/public>
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/RenderEngines/RayCast/public>
              $<INSTALL_INTERFACE:/Sourse/RenderEngines/RayCast/public>
)

target_link_libraries(${PROJECT_NAME} "Types.lib" "RayCast.lib" "d2d1.lib")


file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Configuration)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/Configuration/ ${CMAKE_BINARY_DIR}/bin/Configuration)
