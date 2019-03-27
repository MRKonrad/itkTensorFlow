# see https://github.com/Neargye/hello_tf_c_api
# TODO: fix controvery: for win32 downloading .zip file and renaming it to tar.gz

if(UNIX AND NOT APPLE)
    set(DOWNLOADPATH "https://github.com/Neargye/tensorflow/releases/download/v1.13.1/libtensorflow-cpu-linux-x86_64-1.13.1.tar.gz")
elseif(APPLE)
    set(DOWNLOADPATH "https://github.com/Neargye/tensorflow/releases/download/v1.13.1/libtensorflow-cpu-darwin-x86_64-1.13.1.tar.gz")
elseif(WIN32)
    set(DOWNLOADPATH "https://github.com/Neargye/tensorflow/releases/download/v1.13.1/libtensorflow-cpu-windows-x86_64-1.13.1.zip")
endif()

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow)
    make_directory(${CMAKE_SOURCE_DIR}/thirdParty/tensorflow)
endif()

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/libtensorflow.so)
    file(
            DOWNLOAD
                ${DOWNLOADPATH}
                ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/libtensorflow.tar.gz
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

    execute_process(
            COMMAND cmake -E tar xvzf ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/libtensorflow.tar.gz
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow)
    file(REMOVE ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/libtensorflow.tar.gz)

    include_directories(${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/include/tensorflow/c)
    link_directories(${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib)

endif()

if(WIN32)
    configure_file(tensorflow/lib/tensorflow.dll ${CMAKE_CURRENT_BINARY_DIR}/tensorflow.dll COPYONLY)
    configure_file(tensorflow/lib/tensorflow.dll ${CMAKE_CURRENT_BINARY_DIR}/test/tensorflow.dll COPYONLY)
endif()

