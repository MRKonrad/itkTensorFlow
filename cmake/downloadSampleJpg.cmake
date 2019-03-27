# https://colab.research.google.com/github/tensorflow/models/blob/master/research/deeplab/deeplab_demo.ipynb#scrollTo=edGukUHXyymr

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tests/testData/jpg)
    make_directory(${CMAKE_SOURCE_DIR}/tests/testData/jpg)
endif()


if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tests/testData/jpg/image2.jpg)
    file(
        DOWNLOAD
            https://github.com/tensorflow/models/blob/master/research/deeplab/g3doc/img/image2.jpg?raw=true
            ${CMAKE_SOURCE_DIR}/tests/testData/jpg/image2.jpg
        STATUS status
        LOG log)
    list(GET status 0 status_code)
    list(GET status 1 status_string)

    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "error downloading sample image2.jpg lib: ${status_string}" "${log}")
    endif()

endif()