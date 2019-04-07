#!/bin/bash

../cmake-build-debug/itkTensorFlowExe \
    -r "../tests/testData/jpg/image2" \
    -m "../tests/testData/model_deeplab.pb" \
    -o "../tests/testData/temp/PipelineBuilder_exe"