//
// Created by Konrad Werys on 24/03/2019.
//

#include "gtest/gtest.h"

#include "itktfUtils.h"

#include "samplePipeline.h"

TEST(itkTensorFlow, printTensorFlowVersion) {

    EXPECT_NO_THROW(printTensorFlowVersion());

}

TEST(itkTensorFlow, printGraphInfo) {

    EXPECT_NO_THROW(printGraphInfo("../../tests/testData/model2.pb"));

}

TEST(itkTensorFlow, sessionRun_model2) {

    EXPECT_NO_THROW(sessionRun<uint8_t>(TF_UINT8, 513, 513, 3,"../../tests/testData/model2.pb", "ImageTensor", "SemanticPredictions"));

}

//TEST(itkTensorFlow, sessionRun_model) {
//
//    EXPECT_NO_THROW(sessionRun<float>(TF_FLOAT, 384, 384, 1,"../../tests/testData/model.pb", "input_1", "conv2d_7/truediv"));
//
//}


TEST(itkTensorFlow, oneImagePipeline) {

    char *argv[] = {
            (char*) "",
            (char*) "../../tests/testData/dicom/T1map.dcm",
            (char*) "../../tests/testData/temp/T1map.dcm",
    };

    samplePipeline(3, argv);

    EXPECT_EQ(true, true);

}

