# see https://github.com/Neargye/hello_tf_c_api

if(UNIX AND NOT APPLE)
    set(DownloadUrl "https://github.com/Neargye/tensorflow/releases/download/v1.13.1/libtensorflow-cpu-linux-x86_64-1.13.1.tar.gz")
    set(DownloadPath ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/libtensorflow.tar.gz)
elseif(APPLE)
    set(DownloadUrl "https://github.com/Neargye/tensorflow/releases/download/v1.13.1/libtensorflow-cpu-darwin-x86_64-1.13.1.tar.gz")
    set(DownloadPath ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/libtensorflow.tar.gz)
elseif(WIN32)
    set(DownloadUrl "https://github.com/Neargye/tensorflow/releases/download/v1.13.1/libtensorflow-cpu-windows-x86_64-1.13.1.zip")
    set(DownloadPath ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/libtensorflow.zip)
endif()

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow)
    make_directory(${CMAKE_SOURCE_DIR}/thirdParty/tensorflow)
endif()

if (
  NOT EXISTS ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/libtensorflow.so AND
  NOT EXISTS ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib/tensorflow.dll )

    file( DOWNLOAD ${DownloadUrl} ${DownloadPath} STATUS status LOG log)

    list(GET status 0 status_code)
    list(GET status 1 status_string)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "error downloading tensorflow lib: ${status_string}" "${log}")
    endif()

    execute_process(
            COMMAND cmake -E tar xvzf ${DownloadPath}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/thirdParty/tensorflow)
    file(REMOVE ${DownloadPath})

endif()

include_directories(${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/include/tensorflow/c)
link_directories(${CMAKE_SOURCE_DIR}/thirdParty/tensorflow/lib)

if(WIN32)
    configure_file(thirdParty/tensorflow/lib/tensorflow.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug/tensorflow.dll COPYONLY)
    configure_file(thirdParty/tensorflow/lib/tensorflow.dll ${CMAKE_CURRENT_BINARY_DIR}/tests/Debug/tensorflow.dll COPYONLY)
    configure_file(thirdParty/tensorflow/lib/tensorflow.dll ${CMAKE_CURRENT_BINARY_DIR}/Release/tensorflow.dll COPYONLY)
    configure_file(thirdParty/tensorflow/lib/tensorflow.dll ${CMAKE_CURRENT_BINARY_DIR}/tests/Release/tensorflow.dll COPYONLY)
endif()
