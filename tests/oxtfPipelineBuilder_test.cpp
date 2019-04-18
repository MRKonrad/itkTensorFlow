//
// Created by Konrad Werys on 04/04/2019.
//

#include "gtest/gtest.h"
#include "oxtfPipelineBuilder.h"
#include "itkImage.h"
#include "itkRGBPixel.h"

TEST(PipelineBuilder, PipelineBuilder_noImagePaths) {

    oxtf::PipelineBuilder pipelineBuilder;

    EXPECT_NE(pipelineBuilder.runPipeline(), 0);

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

// TODO: test if it is flipping for real
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

// TODO: test if it is thresholding for real
TEST(PipelineBuilder, PipelineBuilder_thresholdImage) {

    typedef itk::Image<float, 3> ImageType;

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
    float threshold = 100;

    oxtf::PipelineBuilder pipelineBuilder;

    typename ImageType::Pointer image = pipelineBuilder.readInputImageRgb<ImageType>(inputFilename);
    image = pipelineBuilder.thresholdImage<ImageType>(image, threshold);

    EXPECT_NE(image, nullptr);

}

TEST(PipelineBuilder, PipelineBuilder_runGraphOnImage) {

    typedef itk::Image<float, 3> ImageType;

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
    std::string graphFilename = "../../tests/testData/model_deeplab.pb";
    int newX = 513;
    int newY = 513;

    oxtf::PipelineBuilder pipelineBuilder;

    oxtf::GraphReader *graphReader = pipelineBuilder.graphReaderMaker(graphFilename);

    typename ImageType::Pointer image = pipelineBuilder.readInputImageRgb<ImageType>(inputFilename);
    image = pipelineBuilder.padImage<ImageType>(image, newX, newY);
    typename ImageType::Pointer imageOut = pipelineBuilder.runGraphOnImage<ImageType>(image, graphReader);

    EXPECT_NE(imageOut, nullptr);
    EXPECT_EQ(imageOut->GetLargestPossibleRegion().GetSize()[2], 1);

    delete graphReader;

}

TEST(PipelineBuilder, PipelineBuilder_writeImages) {

    typedef itk::Image<float, 3> ImageType;

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
    std::string outputDir = "../../tests/testData/temp/PipelineBuilder";
    std::string graphFilename = "../../tests/testData/model_deeplab.pb";

    oxtf::PipelineBuilder pipelineBuilder;

    oxtf::GraphReader *graphReader = pipelineBuilder.graphReaderMaker(graphFilename);

    typename ImageType::Pointer image = pipelineBuilder.readInputImageRgb<ImageType>(inputFilename);
    typename ImageType::Pointer imageOut = pipelineBuilder.runGraphOnImage<ImageType>(image, graphReader);
    pipelineBuilder.writeImages(imageOut.GetPointer(), outputDir);

    EXPECT_NE(imageOut, nullptr);

    delete graphReader;

}

TEST(PipelineBuilder, PipelineBuilder_runPipeline) {

    typedef itk::Image<float, 3> ImageType;

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
    std::string outputDir = "../../tests/testData/temp/PipelineBuilder2";
    std::string graphFilename = "../../tests/testData/model_deeplab.pb";

    oxtf::PipelineBuilder pipelineBuilder;
    pipelineBuilder.setInputImageRgbPath(inputFilename);
    pipelineBuilder.setOutputDirPath(outputDir);
    pipelineBuilder.setGraphPath(graphFilename);

    EXPECT_EQ(pipelineBuilder.runPipeline(), 0);

}

TEST(PipelineBuilder, PipelineBuilder_runPipeline2) {

    typedef itk::Image<float, 3> ImageType;

    std::vector<std::string> inputFilenames;
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    std::string outputDir = "../../tests/testData/temp/PipelineBuilder3";
    std::string graphFilename = "../../tests/testData/model_deeplab.pb";

    oxtf::PipelineBuilder pipelineBuilder;
    pipelineBuilder.setInputImagesGrayscalePaths(inputFilenames);
    pipelineBuilder.setOutputDirPath(outputDir);
    pipelineBuilder.setGraphPath(graphFilename);
    pipelineBuilder.setMultiplyOutputByFactor(255);

    std::vector<bool> flipAxes = std::vector<bool>(3, false);
    flipAxes[1] = true;
    pipelineBuilder.setFlipAxes(flipAxes);
    pipelineBuilder.setThreshold(255);

    EXPECT_EQ(pipelineBuilder.runPipeline(), 0);

}


/**
 * testing a private model
 */
TEST(PipelineBuilder, PipelineBuilder_runPipeline_moco) {

    typedef itk::Image<float, 3> ImageType;

    std::vector<std::string> inputFilenames;
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    std::string outputDir = "../../tests/testData/temp/PipelineBuilder_moco";
    std::string graphFilename = "../../tests/testData/MoCoAI.pb"; // a private model

    oxtf::PipelineBuilder pipelineBuilder;
    pipelineBuilder.setInputImagesGrayscalePaths(inputFilenames);
    pipelineBuilder.setOutputDirPath(outputDir);
    pipelineBuilder.setGraphPath(graphFilename);
    pipelineBuilder.setMultiplyOutputByFactor(1);

    std::vector<bool> flipAxes = std::vector<bool>(3, false);
    flipAxes[1] = true;
    pipelineBuilder.setFlipAxes(flipAxes);
    pipelineBuilder.setThreshold(255);

    int result = pipelineBuilder.runPipeline();

    if (result == 2) return; // a private model, so we do not want CI to raise warnings

    EXPECT_EQ(result, 0); // test only if the model is available

}

/**
 * testing a private model
 */
TEST(PipelineBuilder, PipelineBuilder_runPipeline_image_size_different_than_graph_size) {

    typedef itk::Image<uint8_t , 3> ImageType;

    std::string inputFilename = "../../tests/testData/dicom/Volunteer_T1Map.dcm";
    std::string outputDir = "../../tests/testData/temp/PipelineBuilder3";
    std::string graphFilename = "../../tests/testData/model_ocmr7.pb"; // a private model

    oxtf::PipelineBuilder pipelineBuilder;
    pipelineBuilder.setInputImageRgbPath(inputFilename);
    pipelineBuilder.setOutputDirPath(outputDir);
    pipelineBuilder.setGraphPath(graphFilename);

    int result = pipelineBuilder.runPipeline();

    if (result == 2) return; // a private model, so we do not want CI to raise warnings

    EXPECT_EQ(result, 1); // 3rd dim of image = 3 3rd dim of graph = 1, should not work

}

/**
 * testing a private model
 */
TEST(PipelineBuilder, PipelineBuilder_runPipeline4) {

    typedef itk::Image<float, 3> ImageType;

    std::vector<std::string> inputFilenames;
    inputFilenames.emplace_back( "../../tests/testData/dicom/T1Map.dcm");
    std::string outputDir = "../../tests/testData/temp/PipelineBuilder4";
    std::string graphFilename = "../../tests/testData/model_ocmr7.pb"; // a private model

    oxtf::PipelineBuilder pipelineBuilder;
    pipelineBuilder.setInputImagesGrayscalePaths(inputFilenames);
    pipelineBuilder.setOutputDirPath(outputDir);
    pipelineBuilder.setGraphPath(graphFilename);
    pipelineBuilder.setMultiplyOutputByFactor(255);

    std::vector<bool> flipAxes = std::vector<bool>(3, false);
    flipAxes[1] = true;
    pipelineBuilder.setFlipAxes(flipAxes);
    pipelineBuilder.setThreshold(2500);

    int result = pipelineBuilder.runPipeline();

    if (result == 2) return; // a private model, so we do not want CI to raise warnings

    EXPECT_EQ(result, 0);

}