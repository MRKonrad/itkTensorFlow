# see https://github.com/Neargye/hello_tf_c_api

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tensorflow)
    make_directory(${CMAKE_SOURCE_DIR}/tensorflow)
endif()

if(UNIX AND NOT APPLE)

    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tensorflow/lib/libtensorflow.so)
        file(
                DOWNLOAD
                    https://github.com/Neargye/tensorflow/releases/download/v1.13.1/libtensorflow-cpu-linux-x86_64-1.13.1.tar.gz ${CMAKE_SOURCE_DIR}/tensorflow/linux.tar.gz
                STATUS
                    status
                LOG
                    log
        )
        list(GET status 0 status_code)
        list(GET status 1 status_string)
        if(NOT status_code EQUAL 0)
            message(FATAL_ERROR "error downloading tensorflow lib: ${status_string}" "${log}")
        endif()

        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvzf ${CMAKE_SOURCE_DIR}/tensorflow/linux.tar.gz WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tensorflow)
        file(REMOVE ${CMAKE_SOURCE_DIR}/tensorflow/linux.tar.gz)
    endif()

    include_directories(${CMAKE_SOURCE_DIR}/tensorflow/include/tensorflow/c)
    link_directories(${CMAKE_SOURCE_DIR}/tensorflow/lib)

elseif(APPLE)

    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tensorflow/lib/libtensorflow.so)
        file(
                DOWNLOAD
                    https://github.com/Neargye/tensorflow/releases/download/v1.13.1/libtensorflow-cpu-darwin-x86_64-1.13.1.tar.gz ${CMAKE_SOURCE_DIR}/tensorflow/darwin.tar.gz
                STATUS
                    status
                LOG
                    log
        )
        list(GET status 0 status_code)
        list(GET status 1 status_string)

        if(NOT status_code EQUAL 0)
            message(FATAL_ERROR "error downloading tensorflow lib: ${status_string}" "${log}")
        endif()

        # KW: removed ${CMAKE_COMMAND} -E
        # message("${CMAKE_COMMAND} -E tar xvzf ${CMAKE_SOURCE_DIR}/tensorflow/darwin.tar.gz WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tensorflow")
        execute_process(COMMAND tar xvzf ${CMAKE_SOURCE_DIR}/tensorflow/darwin.tar.gz WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tensorflow)
        file(REMOVE ${CMAKE_SOURCE_DIR}/tensorflow/darwin.tar.gz)
    endif()

    include_directories(${CMAKE_SOURCE_DIR}/tensorflow/include/tensorflow/c)
    link_directories(tensorflow/lib)

elseif(WIN32)

    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tensorflow/lib/tensorflow.dll)
        file(
                DOWNLOAD
                    https://github.com/Neargye/tensorflow/releases/download/v1.13.1/libtensorflow-cpu-windows-x86_64-1.13.1.zip ${CMAKE_SOURCE_DIR}/tensorflow/windows.zip
                STATUS
                    status
                LOG
                    log
        )
        list(GET status 0 status_code)
        list(GET status 1 status_string)
        if(NOT status_code EQUAL 0)
            message(FATAL_ERROR "error downloading tensorflow lib: ${status_string}" "${log}")
        endif()

        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvzf ${CMAKE_SOURCE_DIR}/tensorflow/windows.zip WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tensorflow)
        file(REMOVE ${CMAKE_SOURCE_DIR}/tensorflow/windows.zip)
    endif()

    include_directories(${CMAKE_SOURCE_DIR}/tensorflow/include/tensorflow/c)
    link_directories(tensorflow/lib)
    configure_file(tensorflow/lib/tensorflow.dll ${CMAKE_CURRENT_BINARY_DIR}/tensorflow.dll COPYONLY)
    configure_file(tensorflow/lib/tensorflow.dll ${CMAKE_CURRENT_BINARY_DIR}/test/tensorflow.dll COPYONLY)
endif()