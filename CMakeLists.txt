cmake_minimum_required(VERSION 3.6)

project(itkTensorFlow)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INSTALL_RPATH "@executable_path")

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
add_library(itkTensorFlowLibStatic STATIC  ${SRC_FILES})
target_link_libraries (itkTensorFlowLib PUBLIC ${ITK_LIBRARIES} tensorflow)

include(GenerateExportHeader)
generate_export_header(itkTensorFlowLib)

add_executable(itkTensorFlowExe app/main.cpp app/getopt.cpp app/getopt.h)
target_link_libraries (itkTensorFlowExe PUBLIC itkTensorFlowLib )

install (TARGETS itkTensorFlowExe DESTINATION .)
install (TARGETS itkTensorFlowLib DESTINATION .)
if(WIN32)
  install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/tensorflow.dll DESTINATION .)
  install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/tensorflow.lib DESTINATION .)
  install (FILES ${CMAKE_SOURCE_DIR}/scripts/sampleScript.bat DESTINATION .)
else()
  install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/libtensorflow.so DESTINATION .)
  install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/libtensorflow_framework.so DESTINATION .)
  install (FILES ${CMAKE_SOURCE_DIR}/scripts/sampleScript.sh DESTINATION .)
endif()

add_subdirectory(tests)
