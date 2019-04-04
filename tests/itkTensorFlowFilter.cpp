//
// Created by Konrad Werys on 04/04/2019.
//

#include "gtest/gtest.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFileTools.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkTileImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkConstantPadImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkFlipImageFilter.h"
#include "itkThresholdImageFilter.h"

#include "itkTensorFlowFilter.h"

#include <string>

TEST(itkTensorFlowFilter, itkTensorFlowFilter_incorrectInputType_shouldThrowError) {

    std::string graphFilename = "../../tests/testData/model_deeplab.pb";

    oxtf::GraphReader graphReader;
    graphReader.setGraphPath(graphFilename);
    graphReader.readGraph();

    typedef itk::Image< std::float_t, 3 > ImageTypeIn;
    typedef itk::Image< std::uint64_t, 2 > ImageTypeOut;
    ImageTypeIn::Pointer imageIn = ImageTypeIn::New();

    ImageTypeIn::IndexType start;
    start.Fill(0);

    ImageTypeIn::SizeType  size;
    size[0] = 100;  // size along X
    size[1] = 200;  // size along Y
    size[2] = 3;    // size along Z

    ImageTypeIn::RegionType region;
    region.SetSize( size );
    region.SetIndex( start );

    imageIn->SetRegions( region );
    imageIn->Allocate();

    typedef itk::TensorFlowImageFilter <ImageTypeIn, ImageTypeOut> TensorFlowImageFilterType;
    TensorFlowImageFilterType::Pointer tfFilter = TensorFlowImageFilterType::New();
    tfFilter->SetInput(imageIn);
    tfFilter->SetGraphReader(&graphReader);
    EXPECT_THROW(tfFilter->Update(), std::runtime_error);
    ImageTypeOut::Pointer imageOut = tfFilter->GetOutput();

}

TEST(itkTensorFlowFilter, itkTensorFlowFilter_incorrectOutputType_shouldThrowError) {

    std::string graphFilename = "../../tests/testData/model_deeplab.pb";

    oxtf::GraphReader graphReader;
    graphReader.setGraphPath(graphFilename);
    graphReader.readGraph();

    typedef itk::Image< std::uint8_t, 3 > ImageTypeIn;
    typedef itk::Image< std::float_t, 2 > ImageTypeOut;
    ImageTypeIn::Pointer imageIn = ImageTypeIn::New();

    ImageTypeIn::IndexType start;
    start.Fill(0);

    ImageTypeIn::SizeType  size;
    size[0] = 100;  // size along X
    size[1] = 200;  // size along Y
    size[2] = 3;    // size along Z

    ImageTypeIn::RegionType region;
    region.SetSize( size );
    region.SetIndex( start );

    imageIn->SetRegions( region );
    imageIn->Allocate();

    typedef itk::TensorFlowImageFilter <ImageTypeIn, ImageTypeOut> TensorFlowImageFilterType;
    TensorFlowImageFilterType::Pointer tfFilter = TensorFlowImageFilterType::New();
    tfFilter->SetInput(imageIn);
    tfFilter->SetGraphReader(&graphReader);
    EXPECT_THROW(tfFilter->Update(), std::runtime_error);
    ImageTypeOut::Pointer imageOut = tfFilter->GetOutput();

}

//TODO: implement
//TEST(itkTensorFlowFilter, itkTensorFlowFilter_incorrectInputDims_shouldThrowError) {
//
//    EXPECT_EQ(true, false);
//
//}
//
//TEST(itkTensorFlowFilter, itkTensorFlowFilter_incorrectOutputDims_shouldThrowError) {
//
//    EXPECT_EQ(true, false);
//
//}
//
//TEST(itkTensorFlowFilter, itkTensorFlowFilter_incorrectInputSize_shouldThrowError) {
//
//    EXPECT_EQ(true, false);
//
//}
//
//TEST(itkTensorFlowFilter, itkTensorFlowFilter_incorrectOutputSize_shouldThrowError) {
//
//    EXPECT_EQ(true, false);
//
//}