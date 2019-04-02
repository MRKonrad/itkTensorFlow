//
// Created by Konrad Werys on 01/04/2019.
//

#include "gtest/gtest.h"

#include "oxtfGraphRunner.h"

TEST(GraphRunner, GraphRunner_test) {

    std::string graphPath = "../../tests/testData/model2.pb";

    oxtf::GraphReader graphReader;
    graphReader.setGraphPath(graphPath);
    graphReader.readGraph();

    long sizeX = 513, sizeY = 513, sizeZ = 1;
    const std::vector<std::int64_t> input_dims = {1, sizeX, sizeY, sizeZ};
    const std::vector<uint8_t> input_vals ((unsigned long)sizeX * sizeY * sizeZ, 1);

    TF_Tensor* inputTensor = tf_utils::CreateTensor(TF_UINT8,
                                                     input_dims.data(), input_dims.size(),
                                                     input_vals.data(), input_vals.size() * sizeof(uint8_t));

    oxtf::GraphRunner graphRunner;
    graphRunner.setGraphReader(&graphReader);
    graphRunner.setInputTensor(inputTensor);

    // process
    EXPECT_NO_THROW(graphRunner.run());
    TF_Tensor* outputTensor = graphRunner.getOutputTensor();

    // tidy up
    TF_DeleteTensor(inputTensor);
    TF_DeleteTensor(outputTensor);

}
