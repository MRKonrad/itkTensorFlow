cmake_minimum_required(VERSION 3.6)

project(itkTensorFlow)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/tensorflow.cmake)

find_package(ITK REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.c* src/*.h* src/*.t*)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_library(itkTensorFlowLib ${SRC_FILES})

add_subdirectory(tests)



