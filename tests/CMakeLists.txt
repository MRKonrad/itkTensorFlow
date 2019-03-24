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
#set_target_properties(TomatoTests PROPERTIES COMPILE_FLAGS -DTomatoLib_EXPORTS) # to see hxx content

add_test(NAME ctest_tests COMMAND itkTensorFlowTests)

# download sample model
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tests/testData/model.pb)
    file(
            DOWNLOAD
                http://download.tensorflow.org/models/deeplabv3_mnv2_dm05_pascal_trainaug_2018_10_01.tar.gz
                ${CMAKE_SOURCE_DIR}/tests/testData/model.tar.gz
            STATUS status
            LOG log)
    list(GET status 0 status_code)
    list(GET status 1 status_string)

    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "error downloading tensorflow lib: ${status_string}" "${log}")
    endif()

    execute_process(
            COMMAND cmake -E tar xvzf ${CMAKE_SOURCE_DIR}/tests/testData/model.tar.gz
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/testData)
    execute_process(
            COMMAND cmake -E copy ${CMAKE_SOURCE_DIR}/tests/testData/deeplabv3_mnv2_dm05_pascal_trainaug/frozen_inference_graph.pb ${CMAKE_SOURCE_DIR}/tests/testData/model.pb
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/testData)

    file(REMOVE ${CMAKE_SOURCE_DIR}/tests/testData/model.tar.gz)
    file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/tests/testData/deeplabv3_mnv2_dm05_pascal_trainaug)
endif()