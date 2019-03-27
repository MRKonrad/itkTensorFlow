include(../cmake/gtest.cmake)

enable_testing()

# Threads are needed for testing
find_package(Threads REQUIRED)
target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})

# Test executable
file(GLOB_RECURSE TESTS_FILES ${PROJECT_SOURCE_DIR}/tests/*.c* ${PROJECT_SOURCE_DIR}/tests/*.h* ${PROJECT_SOURCE_DIR}/tests/*.t*)

add_executable(itkTensorFlowTests ${TESTS_FILES})
target_link_libraries(itkTensorFlowTests PUBLIC gtest)
target_link_libraries(itkTensorFlowTests PUBLIC itkTensorFlowLib tensorflow)
#set_target_properties(itkTensorFlowTests PROPERTIES COMPILE_FLAGS -DitkTensorFlow_EXPORTS) # to see hxx content

add_test(NAME ctest_tests COMMAND itkTensorFlowTests)

include(../cmake/dowloadSampleModel.cmake)
include(../cmake/downloadSampleJpg.cmake)