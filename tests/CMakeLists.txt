include(${PROJECT_SOURCE_DIR}/cmake/gtest.cmake)

enable_testing()

# Threads are needed for testing
find_package(Threads REQUIRED)
target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})

# Test executable
file(GLOB_RECURSE TESTS_FILES ${PROJECT_SOURCE_DIR}/tests/*.c* ${PROJECT_SOURCE_DIR}/tests/*.h* ${PROJECT_SOURCE_DIR}/tests/*.t*)

add_executable(itkTensorFlowTests ${TESTS_FILES})
target_link_libraries(itkTensorFlowTests PUBLIC gtest)
target_link_libraries(itkTensorFlowTests PUBLIC tensorflow)
target_link_libraries(itkTensorFlowTests PUBLIC ${ITK_LIBRARIES})
target_link_libraries(itkTensorFlowTests PUBLIC itkTensorFlowLib)

add_test(NAME ctest_tests COMMAND itkTensorFlowTests)

include(${PROJECT_SOURCE_DIR}/cmake/dowloadSampleModel.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/downloadSampleJpg.cmake)

if(WIN32)
  file(COPY testData DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testData/temp)
endif()

install (TARGETS itkTensorFlowTests DESTINATION tests)
install (DIRECTORY ${CMAKE_SOURCE_DIR}/tests/testData DESTINATION .)

if(WIN32)
  install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/tensorflow.dll DESTINATION tests)
  install (FILES ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/lib/itkTensorFlowLib.dll DESTINATION tests)
else()
  install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/libtensorflow.so DESTINATION tests)
  install (FILES ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/libtensorflow_framework.so DESTINATION tests)
  install (FILES ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/lib/libitkTensorFlowLib${CMAKE_SHARED_LIBRARY_SUFFIX} DESTINATION tests)
endif()
