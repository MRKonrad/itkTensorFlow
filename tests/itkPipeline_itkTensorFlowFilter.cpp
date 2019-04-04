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

TEST(itkPipeline_itkTensorFlowFilter, itkTensorFlowFilter_test) {

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
    std::string outputFilename = "../../tests/testData/temp/itkTensorFlowFilter_test.png";
    std::string graphFilename = "../../tests/testData/model_deeplab.pb";

    oxtf::GraphReader graphReader;
    graphReader.setGraphPath(graphFilename);
    graphReader.readGraph();

    typedef itk::RGBPixel<unsigned char> RgbPixelType;
    typedef unsigned char GrayPixelTypeIn;
    typedef std::int64_t GrayPixelTypeOut;
    typedef unsigned char PixelTypeOut;
    typedef itk::Image<RgbPixelType, 2> RgbImageType;
    typedef itk::Image<GrayPixelTypeIn, 3> GrayImageTypeIn;
    typedef itk::Image<GrayPixelTypeOut, 2> GrayImageTypeOut;
    typedef itk::Image<PixelTypeOut, 2> ImageTypeOut;

    typedef itk::ImageFileReader<RgbImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFilename);
    reader->Update();

    typedef itk::ImageRegion< 2 > RegionType;
    RegionType region = reader->GetOutput()->GetLargestPossibleRegion();

    RgbImageType::SizeType lowerExtendRegion;
    lowerExtendRegion[0] = 0;
    lowerExtendRegion[1] = 0;

    RgbImageType::SizeType upperExtendRegion;
    upperExtendRegion[0] = 513 - region.GetSize()[0];
    upperExtendRegion[1] = 513 - region.GetSize()[1];

    RgbPixelType zeroPixel;
    zeroPixel.SetRed(0);
    zeroPixel.SetGreen(0);
    zeroPixel.SetBlue(0);

    typedef itk::ConstantPadImageFilter< RgbImageType, RgbImageType > ConstantPadType;
    ConstantPadType::Pointer constantPad = ConstantPadType::New();
    constantPad->SetPadLowerBound(lowerExtendRegion);
    constantPad->SetPadUpperBound(upperExtendRegion);
    constantPad->SetConstant( zeroPixel );
    constantPad->SetInput(reader->GetOutput());
    constantPad->Update();

    //***********************
    //*** RGB to 3d start ***
    //***********************

    typedef itk::VectorIndexSelectionCastImageFilter<RgbImageType, GrayImageTypeIn> ExtractFromVectorFilterType;
    ExtractFromVectorFilterType::Pointer extractFromVectorFilter = ExtractFromVectorFilterType::New();
    extractFromVectorFilter->SetInput(constantPad->GetOutput());

    typedef itk::TileImageFilter<GrayImageTypeIn, GrayImageTypeIn> TileImageFilterType;
    TileImageFilterType::Pointer tiler = TileImageFilterType::New();
    itk::FixedArray<unsigned int, 3> layout;
    layout[0] = 1;
    layout[1] = 1;
    layout[2] = 0;
    tiler->SetLayout(layout);

    RgbPixelType tempPixel;
    for (unsigned int i = 0; i < tempPixel.Size(); i++) {
        extractFromVectorFilter->SetIndex(i);
        extractFromVectorFilter->Update();
        GrayImageTypeIn::Pointer input = extractFromVectorFilter->GetOutput();
        input->DisconnectPipeline();
        tiler->SetInput(i, input);
    }
    tiler->Update();

    //***********************
    //*** RGB to 3d stop  ***
    //***********************

    //*******************************************
    //*** What we actually want to test start ***
    //*******************************************

    typedef itk::TensorFlowImageFilter <GrayImageTypeIn, GrayImageTypeOut> TensorFlowImageFilterType;
    TensorFlowImageFilterType::Pointer tfFilter = TensorFlowImageFilterType::New();
    tfFilter->SetInput(tiler->GetOutput());
    tfFilter->SetGraphReader(&graphReader);
    EXPECT_NO_THROW(tfFilter->Update());
    GrayImageTypeOut::Pointer image = tfFilter->GetOutput();

    //*******************************************
    //*** What we actually want to test stop  ***
    //*******************************************

    typedef  itk::RescaleIntensityImageFilter< GrayImageTypeOut, ImageTypeOut  > RescaleIntensityType;
    RescaleIntensityType::Pointer rescaleIntensity = RescaleIntensityType::New();
    rescaleIntensity->SetInput(image);
    rescaleIntensity->Update();

    auto pos = outputFilename.rfind(fileSeparator());
    if (pos!= std::string::npos) {
        itk::FileTools::CreateDirectory(outputFilename.substr(0, pos));
    }

    typedef itk::ImageFileWriter<ImageTypeOut> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(rescaleIntensity->GetOutput());
    writer->Update();

}

