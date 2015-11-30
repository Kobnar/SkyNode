# CMAKE:
#-------

cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# PROJECT:
#---------

project(SkyNode CXX)


# VERSION:
#---------

set (SkyNode_VERSION_MAJOR 0)
set (SkyNode_VERSION_MINOR 1)


# DEPENDENCIES:
#-----------

#set (CMAKE_SHARED_LINKER_FLAGS "-lconfig++")

find_package (OpenCV REQUIRED)
include_directories (${OpenCV_INCLUDE_DIRS})


# BUILD PATHS:
#-------------

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


# SOURCE FILES:
#--------------

add_subdirectory(src)
