//
// Created by Konrad Werys on 04/04/2019.
//

#include "gtest/gtest.h"
#include "oxtfPipelineBuilder.h"

TEST(PipelineBuilder, PipelineBuilder_noImagePaths) {

    oxtf::PipelineBuilder pipelineBuilder;

    EXPECT_THROW(pipelineBuilder.runPipeline(), std::runtime_error);

}

TEST(PipelineBuilder, PipelineBuilder_readInputImageRgb) {

    typedef itk::Image<float, 3> ImageType;

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";

    oxtf::PipelineBuilder pipelineBuilder;

    typename ImageType::Pointer image = pipelineBuilder.readInputImageRgb<ImageType>(inputFilename);

    EXPECT_NE(image, nullptr);
    EXPECT_EQ(image->GetLargestPossibleRegion().GetSize()[2], 3);
}

TEST(PipelineBuilder, PipelineBuilder_readInputImageGray_one) {

    typedef itk::Image<float, 3> ImageType;

    std::vector<std::string> inputFilenames;
    inputFilenames.emplace_back("../../tests/testData/dicom/T1Map.dcm");

    oxtf::PipelineBuilder pipelineBuilder;

    typename ImageType::Pointer image = pipelineBuilder.readInputImageGray<ImageType>(inputFilenames);

    EXPECT_NE(image, nullptr);
    EXPECT_EQ(image->GetLargestPossibleRegion().GetSize()[2], 1);
}

TEST(PipelineBuilder, PipelineBuilder_readInputImageGray_many) {

    typedef itk::Image<float, 3> ImageType;

    std::vector<std::string> inputFilenames;
    inputFilenames.emplace_back("../../tests/testData/dicom/T1Map.dcm");
    inputFilenames.emplace_back("../../tests/testData/dicom/T1Map.dcm");

    oxtf::PipelineBuilder pipelineBuilder;

    typename ImageType::Pointer image = pipelineBuilder.readInputImageGray<ImageType>(inputFilenames);

    EXPECT_NE(image, nullptr);
    EXPECT_EQ(image->GetLargestPossibleRegion().GetSize()[2], 2);
}

TEST(PipelineBuilder, PipelineBuilder_padImage) {

    typedef itk::Image<float, 3> ImageType;

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
    int newX = 1000;
    int newY = 1000;

    oxtf::PipelineBuilder pipelineBuilder;

    typename ImageType::Pointer image = pipelineBuilder.readInputImageRgb<ImageType>(inputFilename);
    image = pipelineBuilder.padImage<ImageType>(image, newX, newY);

    EXPECT_NE(image, nullptr);
    EXPECT_EQ(image->GetLargestPossibleRegion().GetSize()[0], newX);
    EXPECT_EQ(image->GetLargestPossibleRegion().GetSize()[1], newY);
    EXPECT_EQ(image->GetLargestPossibleRegion().GetSize()[2], 3);
}

// TODO: test if it is flipping
TEST(PipelineBuilder, PipelineBuilder_flipImage) {

    typedef itk::Image<float, 3> ImageType;

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
    std::vector<bool> flipAxes = std::vector<bool>(3, false);
    flipAxes[1] = true;

    oxtf::PipelineBuilder pipelineBuilder;

    typename ImageType::Pointer image = pipelineBuilder.readInputImageRgb<ImageType>(inputFilename);
    image = pipelineBuilder.flipImage<ImageType>(image, flipAxes);

    EXPECT_NE(image, nullptr);

}

// TODO: test if it is thresholding
TEST(PipelineBuilder, PipelineBuilder_thresholdImage) {

    typedef itk::Image<float, 3> ImageType;

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
    float threshold = 100;

    oxtf::PipelineBuilder pipelineBuilder;

    typename ImageType::Pointer image = pipelineBuilder.readInputImageRgb<ImageType>(inputFilename);
    image = pipelineBuilder.thresholdImage<ImageType>(image, threshold);

    EXPECT_NE(image, nullptr);

}

//TEST(PipelineBuilder, PipelineBuilder_runGraphOnImage) {
//
//    typedef itk::Image<float, 3> ImageType;
//
//    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
//    std::string graphFilename = "../../tests/testData/model_deeplab.pb";
//    int newX = 513;
//    int newY = 513;
//
//    oxtf::PipelineBuilder pipelineBuilder;
//
//    oxtf::GraphReader *graphReader = pipelineBuilder.graphReaderMaker(graphFilename);
//
//    typename ImageType::Pointer image = pipelineBuilder.readInputImageRgb<ImageType>(inputFilename);
//    image = pipelineBuilder.padImage<ImageType>(image, newX, newY);
//    image = pipelineBuilder.runGraphOnImage<ImageType>(image, graphReader);
//
//    EXPECT_NE(image, nullptr);
//
//    delete graphReader;
//
//}
