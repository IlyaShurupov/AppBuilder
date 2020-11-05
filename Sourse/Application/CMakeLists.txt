# Minimum cmake version required
cmake_minimum_required(VERSION 3.8)

# Project configuration
project(Application)

# Types project

#set (TYPES_HEADERS

#)

# Source files
#set (TYPES_SOURCES
#    "Entry.cpp"
#)


# Create the Types library
add_executable("${PROJECT_NAME}" "Entry.cpp")

#install(FILES "" DESTINATION bin)