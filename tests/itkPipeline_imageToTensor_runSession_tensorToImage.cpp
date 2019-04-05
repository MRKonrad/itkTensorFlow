//
// Created by Konrad Werys on 4/2/19.
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

#include "oxtfImageToTensor.h"
#include "oxtfTensorToItkImage.h"
#include "oxtfGraphReader.h"
#include "oxtfGraphRunner.h"

#include <string>

TEST(itkPipeline_imageToTensor_runSession_tensorToImage, pipelineRgb_test) {

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
    std::string outputFilename = "../../tests/testData/temp/itkPipeline_imageToTensor_runSession_tensorToImage_rgb.png";
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

    TF_Tensor *inputTensor;
    oxtf::ImageToTensor<GrayImageTypeIn>(tiler->GetOutput(), &inputTensor);

    oxtf::GraphRunner graphRunner;
    graphRunner.setGraphReader(&graphReader);
    graphRunner.setInputTensor(inputTensor);

    EXPECT_NO_THROW(graphRunner.run());

    TF_Tensor *outputTensor = graphRunner.getOutputTensor();

    GrayImageTypeOut::Pointer image = GrayImageTypeOut::New();
    oxtf::TensorToImage<GrayImageTypeOut>(outputTensor, image);

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

TEST(itkPipeline_imageToTensor_runSession_tensorToImage, pipelineDicom_test) {

    std::string inputFilename = "../../tests/testData/dicom/T1Map.dcm";
    std::string outputFilename = "../../tests/testData/temp/itkPipeline_imageToTensor_runSession_tensorToImage_dicom.dcm";
    std::string graphFilename = "../../tests/testData/model_deeplab.pb";

    oxtf::GraphReader graphReader;
    graphReader.setGraphPath(graphFilename);
    graphReader.readGraph();

    typedef int dcmPixelType;
    typedef unsigned char tfPixelTypeIn;
    typedef std::int64_t tfPixelTypeOut;
    typedef itk::Image<dcmPixelType, 2> dcmImageType;
    typedef itk::Image<tfPixelTypeIn, 2> dcmImageTypeWithTfPixelTypeIn;
    typedef itk::Image<tfPixelTypeIn, 3> tfImageTypeIn;
    typedef itk::Image<tfPixelTypeOut, 2> tfImageTypeOut;


    typedef itk::ImageFileReader<dcmImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFilename);
    reader->Update();

    typedef  itk::RescaleIntensityImageFilter< dcmImageType, dcmImageTypeWithTfPixelTypeIn  > RescaleIntensityType;
    RescaleIntensityType::Pointer rescaleIntensity = RescaleIntensityType::New();
    rescaleIntensity->SetInput(reader->GetOutput());
    rescaleIntensity->Update();

    typedef itk::ImageRegion< 2 > RegionType;
    RegionType region = reader->GetOutput()->GetLargestPossibleRegion();

    dcmImageTypeWithTfPixelTypeIn::SizeType lowerExtendRegion;
    lowerExtendRegion[0] = 0;
    lowerExtendRegion[1] = 0;

    dcmImageTypeWithTfPixelTypeIn::SizeType upperExtendRegion;
    upperExtendRegion[0] = 513 - region.GetSize()[0];
    upperExtendRegion[1] = 513 - region.GetSize()[1];

    dcmPixelType zeroPixel(0);

    typedef itk::ConstantPadImageFilter< dcmImageTypeWithTfPixelTypeIn, dcmImageTypeWithTfPixelTypeIn > ConstantPadType;
    ConstantPadType::Pointer constantPad = ConstantPadType::New();
    constantPad->SetPadLowerBound(lowerExtendRegion);
    constantPad->SetPadUpperBound(upperExtendRegion);
    constantPad->SetConstant( zeroPixel );
    constantPad->SetInput(rescaleIntensity->GetOutput());
    constantPad->Update();

    //***********************
    //*** RGB to 3d start ***
    //***********************

    typedef itk::TileImageFilter<dcmImageTypeWithTfPixelTypeIn, tfImageTypeIn> TileImageFilterType;
    TileImageFilterType::Pointer tiler = TileImageFilterType::New();
    itk::FixedArray<unsigned int, 3> layout;
    layout[0] = 1;
    layout[1] = 1;
    layout[2] = 0;
    tiler->SetLayout(layout);

    tfPixelTypeIn tempPixel;
    for (unsigned int i = 0; i < 3; i++) {
        dcmImageTypeWithTfPixelTypeIn::Pointer input = constantPad->GetOutput();
        input->DisconnectPipeline();
        tiler->SetInput(i, input);
    }
    tiler->Update();

    //***********************
    //*** RGB to 3d stop  ***
    //**********************

    //*******************************************
    //*** What we actually want to test start ***
    //*******************************************

    TF_Tensor *inputTensor;
    oxtf::ImageToTensor<tfImageTypeIn>(tiler->GetOutput(), &inputTensor);

    oxtf::GraphRunner graphRunner;
    graphRunner.setGraphReader(&graphReader);
    graphRunner.setInputTensor(inputTensor);

    EXPECT_NO_THROW(graphRunner.run());

    TF_Tensor *outputTensor = graphRunner.getOutputTensor();

    tfImageTypeOut::Pointer image = tfImageTypeOut::New();
    oxtf::TensorToImage<tfImageTypeOut>(outputTensor, image);

    //*******************************************
    //*** What we actually want to test stop  ***
    //*******************************************

    typedef  itk::RescaleIntensityImageFilter< tfImageTypeOut, dcmImageType  > RescaleIntensityType2;
    RescaleIntensityType2::Pointer rescaleIntensity2 = RescaleIntensityType2::New();
    rescaleIntensity2->SetInput(image);
    rescaleIntensity2->Update();

    auto pos = outputFilename.rfind(fileSeparator());
    if (pos!= std::string::npos) {
        itk::FileTools::CreateDirectory(outputFilename.substr(0, pos));
    }

    typedef itk::ImageFileWriter<dcmImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(rescaleIntensity2->GetOutput());
    writer->Update();

}

TEST(itkPipeline_imageToTensor_runSession_tensorToImage, pipelineDicom2_test) {

    std::string inputFilename = "../../tests/testData/dicom/Volunteer_T1Map.dcm";
    std::string outputFilename = "../../tests/testData/temp/itkPipeline_imageToTensor_runSession_tensorToImage_dicom2";
    std::string graphFilename = "../../tests/testData/model_ocmr7.pb";

    oxtf::GraphReader graphReader;
    graphReader.setGraphPath(graphFilename);
    if (graphReader.readGraph() != 0){ //needs exit success
        return;
    }

    typedef float PixelTypeIn;
    typedef unsigned char PixelTypeOut;
    typedef itk::Image<PixelTypeIn, 2> tfImageTypeIn;
    typedef itk::Image<PixelTypeIn, 3> tfImageTypeOut;
    typedef itk::Image<PixelTypeIn, 2> ImageTypeOutFloat;
    typedef itk::Image<PixelTypeOut, 2> ImageTypeOut;


    typedef itk::ImageFileReader<tfImageTypeIn> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFilename);
    reader->Update();

    typedef itk::ImageRegion< 2 > RegionType;
    RegionType region = reader->GetOutput()->GetLargestPossibleRegion();

    tfImageTypeIn::SizeType lowerExtendRegion;
    lowerExtendRegion[0] = 0;
    lowerExtendRegion[1] = 0;

    tfImageTypeIn::SizeType upperExtendRegion;
    upperExtendRegion[0] = 384 - region.GetSize()[0];
    upperExtendRegion[1] = 384 - region.GetSize()[1];

    PixelTypeIn zeroPixel(0);

    typedef itk::ConstantPadImageFilter< tfImageTypeIn, tfImageTypeIn > ConstantPadType;
    ConstantPadType::Pointer constantPad = ConstantPadType::New();
    constantPad->SetPadLowerBound(lowerExtendRegion);
    constantPad->SetPadUpperBound(upperExtendRegion);
    constantPad->SetConstant( zeroPixel );
    constantPad->SetInput(reader->GetOutput());
    constantPad->Update();

    //***************************
    //*** special sauce start ***
    //***************************

    itk::FixedArray<bool, 2> flipAxes;
    flipAxes[0] = false;
    flipAxes[1] = true;

    typedef itk::FlipImageFilter <tfImageTypeIn> FlipImageFilterType;
    FlipImageFilterType::Pointer flipFilter = FlipImageFilterType::New ();
    flipFilter->SetInput(constantPad->GetOutput());
    flipFilter->SetFlipAxes(flipAxes);
    flipFilter->Update();

    typedef itk::ThresholdImageFilter <tfImageTypeIn> ThresholdImageFilterType;
    ThresholdImageFilterType::Pointer thresholdFilter = ThresholdImageFilterType::New();
    thresholdFilter->SetInput(flipFilter->GetOutput());
    thresholdFilter->ThresholdAbove(2500);
    thresholdFilter->SetOutsideValue(2500);
    thresholdFilter->Update();

    //***************************
    //*** special sauce stop  ***
    //***************************

    //*******************************************
    //*** What we actually want to test start ***
    //*******************************************

    TF_Tensor *inputTensor;
    oxtf::ImageToTensor<tfImageTypeIn>(thresholdFilter->GetOutput(), &inputTensor);

    oxtf::GraphRunner graphRunner;
    graphRunner.setGraphReader(&graphReader);
    graphRunner.setInputTensor(inputTensor);

    EXPECT_NO_THROW(graphRunner.run());

    TF_Tensor *outputTensor = graphRunner.getOutputTensor();

    tfImageTypeOut::Pointer image = tfImageTypeOut::New();
    oxtf::TensorToImage<tfImageTypeOut>(outputTensor, image);

    //*******************************************
    //*** What we actually want to test stop  ***
    //*******************************************

    //***************************
    //*** special sauce start ***
    //***************************

    itk::FixedArray<bool, 3> flipAxes2;
    flipAxes2[0] = flipAxes[0];
    flipAxes2[1] = flipAxes[1];
    flipAxes2[2] = false;

    typedef itk::FlipImageFilter <tfImageTypeOut> FlipImageFilterType2;
    FlipImageFilterType2::Pointer flipFilter2 = FlipImageFilterType2::New ();
    flipFilter2->SetInput(image);
    flipFilter2->SetFlipAxes(flipAxes2);

    //***************************
    //*** special sauce stop  ***
    //***************************

    typedef itk::ExtractImageFilter<tfImageTypeOut, ImageTypeOutFloat> ExtractImageFilterType;
    ExtractImageFilterType::Pointer extractor = ExtractImageFilterType::New();
    extractor->SetInput(flipFilter2->GetOutput());
    extractor->SetDirectionCollapseToIdentity();

    typedef itk::ImageRegion< 3 > RegionType3d;
    RegionType3d region3d = image->GetLargestPossibleRegion();
    RegionType3d::SizeType size = region3d.GetSize();
    RegionType3d::IndexType index = region3d.GetIndex();

    unsigned long nImages = image->GetLargestPossibleRegion().GetSize()[2];
    for (unsigned int i = 0; i < nImages; i++) {
        size[2] = 0;
        index[2] = i;
        region3d.SetSize(size);
        region3d.SetIndex(index);
        extractor->SetExtractionRegion(region3d);

        typedef itk::RescaleIntensityImageFilter<ImageTypeOutFloat, ImageTypeOut> RescaleIntensityType2;
        RescaleIntensityType2::Pointer rescaleIntensity2 = RescaleIntensityType2::New();
        rescaleIntensity2->SetInput(extractor->GetOutput());
        rescaleIntensity2->Update();

        auto pos = outputFilename.rfind(fileSeparator());
        if (pos != std::string::npos) {
            itk::FileTools::CreateDirectory(outputFilename.substr(0, pos));
        }

        typedef itk::ImageFileWriter<ImageTypeOut> WriterType;
        WriterType::Pointer writer = WriterType::New();
        writer->SetFileName(outputFilename + "_" + std::to_string(i) + ".png");
        writer->SetInput(rescaleIntensity2->GetOutput());
        writer->Update();
    }
}
