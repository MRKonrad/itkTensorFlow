# download sample model

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tests/testData)
    make_directory(${CMAKE_SOURCE_DIR}/tests/testData)
endif()

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tests/testData/model2.pb)
    file(
            DOWNLOAD
            #http://download.tensorflow.org/models/deeplabv3_mnv2_dm05_pascal_trainaug_2018_10_01.tar.gz
            http://download.tensorflow.org/models/deeplabv3_mnv2_pascal_train_aug_2018_01_29.tar.gz
            ${CMAKE_SOURCE_DIR}/tests/testData/model2.tar.gz
            STATUS status
            LOG log)
    list(GET status 0 status_code)
    list(GET status 1 status_string)

    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "error downloading tensorflow lib: ${status_string}" "${log}")
    endif()

    execute_process(
            COMMAND cmake -E tar xvzf ${CMAKE_SOURCE_DIR}/tests/testData/model2.tar.gz
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/testData)
    execute_process(
            # COMMAND cmake -E copy ${CMAKE_SOURCE_DIR}/tests/testData/deeplabv3_mnv2_dm05_pascal_trainaug/frozen_inference_graph.pb ${CMAKE_SOURCE_DIR}/tests/testData/model2.pb
            COMMAND cmake -E copy ${CMAKE_SOURCE_DIR}/tests/testData/deeplabv3_mnv2_pascal_train_aug/frozen_inference_graph.pb ${CMAKE_SOURCE_DIR}/tests/testData/model2.pb
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/testData)

    file(REMOVE ${CMAKE_SOURCE_DIR}/tests/testData/model2.tar.gz)
    #file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/tests/testData/deeplabv3_mnv2_dm05_pascal_trainaug)
endif()