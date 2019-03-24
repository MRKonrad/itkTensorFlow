//
// Created by Konrad Werys on 24/03/2019.
//

#include "gtest/gtest.h"

#include "itkTensorFlowFilter.h"

TEST(itkTensorFlow, printTensorFlowVersion) {

    printTensorFlowVersion();
    EXPECT_EQ(true, true);

}

