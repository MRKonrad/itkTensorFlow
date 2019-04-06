cmake_minimum_required(VERSION 3.6)

project(itkTensorFlow)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ITK REQUIRED HINTS ${ITK_DIR_HINTS})
include(${ITK_USE_FILE})

file(GLOB_RECURSE SRC_FILES
        src/*.c*
        src/*.h*
        src/*.t*
        thirdParty/hello_tf_c_api/src/tf_utils.cpp)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/thirdParty/hello_tf_c_api/src)
include_directories(${PROJECT_BINARY_DIR}) # to make itktensorflowlib_export.h visible

include(cmake/tensorflow.cmake)

add_library(itkTensorFlowLib SHARED ${SRC_FILES})
target_link_libraries (itkTensorFlowLib PUBLIC ${ITK_LIBRARIES} tensorflow)

include(GenerateExportHeader)
generate_export_header(itkTensorFlowLib)

add_executable(itkTensorFlowExe app/main.cpp app/getopt.cpp app/getopt.h)
target_link_libraries (itkTensorFlowExe PUBLIC itkTensorFlowLib)

set(TOMATO_LIBS_TO_LINK ${TOMATO_LIBS_TO_LINK} ${ITK_LIBRARIES})

add_subdirectory(tests)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()
