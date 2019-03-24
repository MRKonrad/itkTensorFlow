//
// Created by Konrad Werys on 24/03/2019.
//

#include "gtest/gtest.h"

#include "itkTensorFlowFilter.h"

TEST(itkTensorFlow, printTensorFlowVersion) {

    EXPECT_NO_THROW(printTensorFlowVersion());

}

TEST(itkTensorFlow, printGraphInfo) {

    EXPECT_NO_THROW(printGraphInfo("../../tests/testData/model.pb"));

}

TEST(itkTensorFlow, oneImagePipeline) {

    printTensorFlowVersion();
    EXPECT_EQ(true, true);

}

