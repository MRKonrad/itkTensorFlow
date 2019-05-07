cmake_minimum_required(VERSION 3.6)

project(itkTensorFlow)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INSTALL_RPATH "@executable_path")

message(STATUS "CMAKE_GENERATOR_PLATFORM: " ${CMAKE_GENERATOR_PLATFORM})

find_package(ITK REQUIRED HINTS ${ITK_DIR_HINTS})
include(${ITK_USE_FILE})

file(GLOB_RECURSE SRC_FILES
        src/*.c*
        src/*.h*
        src/*.t*
        thirdParty/hello_tf_c_api/src/tf_utils.cpp)

file(GLOB_RECURSE INCLUDE_FILES src/*.h)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/thirdParty/hello_tf_c_api/src)
include_directories(${PROJECT_BINARY_DIR}) # to make itktensorflowlib_export.h visible

include(cmake/tensorflow.cmake)

add_library(itkTensorFlowLib SHARED ${SRC_FILES})
add_library(itkTensorFlowLibStatic STATIC ${SRC_FILES})

target_link_libraries (itkTensorFlowLib PUBLIC ${ITK_LIBRARIES})
target_link_libraries (itkTensorFlowLib PUBLIC tensorflow)

message(STATUS ${ITK_LIBRARIES})

include(GenerateExportHeader)
generate_export_header(itkTensorFlowLib)

add_executable(itkTensorFlowExe app/main.cpp app/getopt.cpp app/getopt.h)
target_link_libraries (itkTensorFlowExe PUBLIC itkTensorFlowLib )

set(CMAKE_INSTALL_PREFIX install)

install(TARGETS itkTensorFlowExe itkTensorFlowLib itkTensorFlowLibStatic
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib_static)
install(FILES ${INCLUDE_FILES} DESTINATION include)

MESSAGE(${CMAKE_SHARED_LIBRARY_SUFFIX})

if(WIN32)
    install (FILES ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/lib/itkTensorFlowLib.dll DESTINATION bin)
    install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/tensorflow.dll DESTINATION bin)
    install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/tensorflow.dll DESTINATION lib)
#    install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/tensorflow.lib DESTINATION lib)
    install (FILES ${CMAKE_SOURCE_DIR}/scripts/sampleScript.bat DESTINATION bin)
else()
    install (FILES ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/lib/libitkTensorFlowLib${CMAKE_SHARED_LIBRARY_SUFFIX} DESTINATION bin)
    install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/libtensorflow.so DESTINATION bin)
    install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/libtensorflow_framework.so DESTINATION bin)
    install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/libtensorflow.so DESTINATION lib)
    install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/libtensorflow_framework.so DESTINATION lib)
    install (FILES ${CMAKE_SOURCE_DIR}/scripts/sampleScript.sh DESTINATION bin)
endif()

add_subdirectory(tests)
