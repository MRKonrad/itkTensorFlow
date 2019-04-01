//
// Created by Konrad Werys on 01/04/2019.
//

#include "gtest/gtest.h"

#include "oxtfGraphReader.h"

TEST(GraphReader, GraphReader_test) {

    std::string graphPath = "../../tests/testData/model2.pb";

    // ground truth input
    std::string gt_InputOperationName = "ImageTensor";
    std::int64_t gt_InputOperationDims = 4;
    std::vector<std::int64_t> gt_InputOperationSize {1, -1, -1, 3};
    TF_DataType gt_InputOperationType = TF_UINT8;

    std::string gt_OutputOperationName = "SemanticPredictions";
    std::int64_t gt_OutputOperationDims = 3;
    std::vector<std::int64_t> gt_OutputOperationSize {1, -1, -1};
    TF_DataType gt_OutputOperationType = TF_INT64;

    oxtf::GraphReader graphReader;
    graphReader.setGraphPath(graphPath);

    // process
    EXPECT_NO_THROW(graphReader.readGraph());

    // check the values
    EXPECT_EQ(graphReader.getGraphPath(), graphPath);

    EXPECT_EQ(graphReader.getInputOperationName(), gt_InputOperationName);
    EXPECT_EQ(graphReader.getInputOperationDims(), gt_InputOperationDims);
    EXPECT_EQ(graphReader.getInputOperationSize(), gt_InputOperationSize);
    EXPECT_EQ(graphReader.getInputOperationType(), gt_InputOperationType);

    EXPECT_EQ(graphReader.getOutputOperationName(), gt_OutputOperationName);
    EXPECT_EQ(graphReader.getOutputOperationDims(), gt_OutputOperationDims);
    EXPECT_EQ(graphReader.getOutputOperationSize(), gt_OutputOperationSize);
    EXPECT_EQ(graphReader.getOutputOperationType(), gt_OutputOperationType);

}