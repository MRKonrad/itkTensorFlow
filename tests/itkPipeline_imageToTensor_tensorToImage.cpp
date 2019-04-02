//
// Created by Konrad Werys on 4/2/19.
//

#include "gtest/gtest.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFileTools.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkTileImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkComposeImageFilter.h"
#include "itkTestingComparisonImageFilter.h"

#include "itkImageToTensor.h"
#include "tensorToItkImage.h"


TEST(itkPipeline_imageToTensor_tensorToImage, pipelineGrayscale_test){

    std::string inputFilename = "../../tests/testData/dicom/T1Map.dcm";
    std::string outputFilename = "../../tests/testData/temp/itkPipeline_imageToTensor_tensorToImage_gray.dcm";

    typedef int PixelType;
    typedef unsigned short PixelTypeOut;
    typedef itk::Image< PixelType, 2 > ImageType;
    typedef itk::ImageFileReader<ImageType> ReaderType;

    // reading
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFilename);
    reader->Update();

    // the important part 1
    TF_Tensor* tensor;
    itkImageToTensor<ImageType>(reader->GetOutput(), &tensor);

    // the important part 2
    ImageType::Pointer image = ImageType::New();
    tensorToItkImage<ImageType>(tensor, image);

    // for comparison filter
    image->SetOrigin(reader->GetOutput()->GetOrigin());
    image->SetSpacing(reader->GetOutput()->GetSpacing());

    // mkdir
    auto pos = outputFilename.rfind(fileSeparator());
    if (pos!= std::string::npos) {
        itk::FileTools::CreateDirectory(outputFilename.substr(0, pos));
    }

    // writing
    typedef itk::ImageFileWriter<ImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(image);
    writer->Update();

    //***********************
    //*** compare         ***
    //***********************

    typedef itk::Testing::ComparisonImageFilter<ImageType, ImageType> ComparisonImageFilter2dType;
    ComparisonImageFilter2dType::Pointer diff = ComparisonImageFilter2dType::New();
    diff->SetValidInput(reader->GetOutput());
    diff->SetTestInput(image);
    diff->Update();
    diff->UpdateLargestPossibleRegion();

    ASSERT_LE(diff->GetMaximumDifference(), 0);
}

TEST(itkPipeline_imageToTensor_tensorToImage, pipelineRgb_test) {

    std::string inputFilename = "../../tests/testData/jpg/image2.jpg";
    std::string outputFilename = "../../tests/testData/temp/itkPipeline_imageToTensor_tensorToImage_rgb.png";

    typedef itk::RGBPixel< unsigned char > RgbPixelType;
    typedef unsigned char GrayPixelType;
    typedef itk::Image< RgbPixelType, 2 > RgbImageType;
    typedef itk::Image< GrayPixelType, 2 > GrayImageType2d;
    typedef itk::Image< GrayPixelType, 3 > GrayImageType3d;

    typedef itk::ImageFileReader<RgbImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFilename);
    reader->Update();

    //***********************
    //*** RGB to 3d start ***
    //***********************

    typedef itk::VectorIndexSelectionCastImageFilter<RgbImageType, GrayImageType3d> ExtractFromVectorFilterType;
    ExtractFromVectorFilterType::Pointer extractFromVectorFilter = ExtractFromVectorFilterType::New();
    extractFromVectorFilter->SetInput(reader->GetOutput());

    typedef itk::TileImageFilter<GrayImageType3d, GrayImageType3d> TileImageFilterType;
    TileImageFilterType::Pointer tiler = TileImageFilterType::New();
    itk::FixedArray< unsigned int, 3 > layout;
    layout[0] = 1;
    layout[1] = 1;
    layout[2] = 0;
    tiler->SetLayout( layout );

    RgbPixelType tempPixel;
    for (unsigned int i = 0; i < tempPixel.Size(); i++) {
        extractFromVectorFilter->SetIndex(i);
        extractFromVectorFilter->Update();
        GrayImageType3d::Pointer input = extractFromVectorFilter->GetOutput();
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

    TF_Tensor* tensor;
    itkImageToTensor<GrayImageType3d>(tiler->GetOutput(), &tensor);

    GrayImageType3d::Pointer image = GrayImageType3d::New();
    tensorToItkImage<GrayImageType3d>(tensor, image);

    //*******************************************
    //*** What we actually want to test stop  ***
    //*******************************************

    //***********************
    //*** 3d to RGB start ***
    //***********************

    typedef itk::ExtractImageFilter<GrayImageType3d, GrayImageType2d> ExtractFromVolumeFilterType;
    ExtractFromVolumeFilterType::Pointer extractFromVolumeFilter = ExtractFromVolumeFilterType::New();
    extractFromVolumeFilter->SetInput(image);
    extractFromVolumeFilter->SetDirectionCollapseToIdentity();

    typedef itk::ComposeImageFilter<GrayImageType2d, RgbImageType> ComposeImageFilterType;
    ComposeImageFilterType::Pointer composer = ComposeImageFilterType::New();

    GrayImageType3d::SizeType desiredSize = image->GetLargestPossibleRegion().GetSize();
    GrayImageType3d::IndexType desiredIndex = image->GetLargestPossibleRegion().GetIndex();
    for (unsigned int i = 0; i < tempPixel.Size(); i++) {
        desiredSize[2] = 0;
        desiredIndex[2] = i;
        GrayImageType3d::RegionType desiredRegion(desiredIndex, desiredSize);
        extractFromVolumeFilter->SetExtractionRegion(desiredRegion);
        extractFromVolumeFilter->Update();

        GrayImageType2d::Pointer input = extractFromVolumeFilter->GetOutput();
        input->DisconnectPipeline();
        composer->SetInput(i, input);
    }

    //***********************
    //*** 3d to RGB stop  ***
    //***********************

    auto pos = outputFilename.rfind(fileSeparator());
    if (pos!= std::string::npos) {
        itk::FileTools::CreateDirectory(outputFilename.substr(0, pos));
    }

    typedef itk::ImageFileWriter<RgbImageType> WriterTypeRGB;
    WriterTypeRGB::Pointer writer = WriterTypeRGB::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(composer->GetOutput());
    writer->Update();

    //***********************
    //*** compare         ***
    //***********************

    // it would be nice to comapre RGBs, but ComparisonImageFilter does not like rgb pixels
    typedef itk::Testing::ComparisonImageFilter<GrayImageType3d, GrayImageType3d> ComparisonImageFilter2dType;
    ComparisonImageFilter2dType::Pointer diff = ComparisonImageFilter2dType::New();
    diff->SetValidInput(tiler->GetOutput());
    diff->SetTestInput(image);
    diff->Update();
    diff->UpdateLargestPossibleRegion();

    ASSERT_LE(diff->GetMaximumDifference(), 0);
}

