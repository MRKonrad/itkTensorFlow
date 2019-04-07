#!/bin/bash

../cmake-build-debug/itkTensorFlowExe \
    -g "../tests/testData/dicom/T1Map.dcm" \
    -m "../tests/testData/model_deeplab.pb" \
    -o "../tests/testData/temp/PipelineBuilder_exe"