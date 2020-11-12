# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

# Project configuration
project(Application)


set (HEADERS
    "public/Win32API.h"
    "public/Operator.h"
    "public/Seance.h"
    "public/Window.h"
    "public/UI.h"
    "public/KeyMap.h"
)

# Source files
set (SOURCES
    "Window.cpp"
    "Operator.cpp"
    "Seance.cpp"
    "Main.cpp"
    "UI.cpp"
    "Win32API.cpp"
    "KeyMap.cpp"
)


link_directories(${CMAKE_BINARY_DIR}/bin )

# Create proj
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/Types/public>
              $<INSTALL_INTERFACE:/Sourse/Types/public>
              $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sourse/RenderEngines/RayCast/public>
              $<INSTALL_INTERFACE:/Sourse/RenderEngines/RayCast/public>
)
target_link_libraries(${PROJECT_NAME} "Types.lib" "RayCast.lib" "d2d1.lib")

#target_link_libraries (${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Sourse/Types)

#install(TARGETS ${PROJECT_NAME} DESTINATION bin)