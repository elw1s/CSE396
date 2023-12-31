cmake_minimum_required(VERSION 3.10)

project(opencv_demo)

add_executable(server mainserver.cpp server_code1.cpp server_code2.cpp)

set(OPENCV_INCLUDE_DIR /usr/local/include/opencv4/)
set(OPENCV_LIB_DIR /usr/local/lib/)

# Set the relative path to PillowResize.hpp
set(PILLOW_RESIZE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/pillow-resize/include")

message(STATUS "OpenCV library path: ${OPENCV_LIB_DIR}")

# set include directories
target_include_directories(server PUBLIC "${OPENCV_INCLUDE_DIR}" "${PILLOW_RESIZE_INCLUDE_DIR}")

# set library directory
target_link_directories(server PUBLIC "${OPENCV_LIB_DIR}")

# link specific object files we need
target_link_libraries(server PUBLIC opencv_imgcodecs opencv_core opencv_imgproc PillowResize)

find_package(PillowResize REQUIRED)
