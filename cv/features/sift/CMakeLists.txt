cmake_minimum_required(VERSION 3.1)
project(sift)
find_package( OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})
add_executable(sift sift.cpp)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Release)
target_link_libraries(sift ${OpenCV_LIBS})

