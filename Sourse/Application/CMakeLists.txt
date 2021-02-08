# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

# Project configuration
project(Gamuncool)


set (HEADERS
    "Platform/Win32API.h"
    "Core/Print.h"
    "Core/Operator.h"
    "Core/Seance.h"
    "UI/Window.h"
    "UI/UITemplates.h"
    "UI/UInterface.h"
    "UI/UInputsMap.h"
    "UI/UInputs.h" "Platform/SysHandler.h")


# Source files
set (SOURCES
    "UI/private/Window.cpp"
    "Core/private/Operator.cpp"
    "Core/private/Seance.cpp"
    "Core/private/Main.cpp"
    "UI/private/UInterface.cpp"
    "Platform/private/Win32API.cpp"
    "UI/private/UITemplates.cpp"
    "UI/private/UInputsMap.cpp"
    "Core/private/Log.cpp" "Platform/private/SysHandler.cpp")



# Create proj
link_directories(${CMAKE_BINARY_DIR}/bin )
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Types>
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Application>
              $<INSTALL_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/RenderEngines/RayCast>

)

target_link_libraries(${PROJECT_NAME} "Types.lib" "RayCast.lib" "d2d1.lib")


file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Configuration)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/UI/Configuration/ ${CMAKE_BINARY_DIR}/bin/Configuration)

