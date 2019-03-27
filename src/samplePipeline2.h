//
// Created by Konrad Werys on 25/03/2019.
//

#ifndef ITKTENSORFLOW_SAMPLEPIPELINE_H
#define ITKTENSORFLOW_SAMPLEPIPELINE_H

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkConstantPadImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkFileTools.h"
#include "itkGDCMImageIO.h"
#include "itkTensorFlowFilter.h"

int samplePipeline2(int argc, char **argv){

    if(argc < 3)
    {
        std::cerr << "Required: input filename and output filename" << std::endl;

        return EXIT_FAILURE;
    }
    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];

    //using PixelType = itk::RGBPixel< unsigned char >;
    using PixelType = unsigned char ;

    typedef itk::Image< PixelType, 2 > InputImageType;
    typedef itk::Image< PixelType, 2 > FilterImageType;
    typedef itk::Image< PixelType, 2 > OutputImageType;

    typedef itk::ImageFileReader<InputImageType> ReaderType;

    typedef itk::GDCMImageIO           ImageIOType;
    ImageIOType::Pointer gdcmImageIO = ImageIOType::New();

    ReaderType::Pointer reader = ReaderType::New();
//    reader->SetImageIO( gdcmImageIO );
    reader->SetFileName(argv[1]);
    reader->Update();

//    typedef  itk::MinimumMaximumImageCalculator<InputImageType> MinimumMaximumImageCalculatorType;
//    MinimumMaximumImageCalculatorType::Pointer mmFiler = MinimumMaximumImageCalculatorType::New();
//    mmFiler->SetImage(reader->GetOutput());
//    mmFiler->Compute();
//    std::cout << mmFiler->GetMinimum() << " " <<  mmFiler->GetMaximum() << std::endl;

    using RegionType = itk::ImageRegion< 2 >;
    RegionType region = reader->GetOutput()->GetLargestPossibleRegion();

    FilterImageType::SizeType lowerExtendRegion;
    lowerExtendRegion[0] = 0;
    lowerExtendRegion[1] = 0;

    FilterImageType::SizeType upperExtendRegion;
    upperExtendRegion[0] = 513 - region.GetSize()[0];
    upperExtendRegion[1] = 513 - region.GetSize()[1];

    PixelType zeroPixel;
//    zeroPixel.SetRed(0);
//    zeroPixel.SetGreen(0);
//    zeroPixel.SetBlue(0)
    zeroPixel = 0;

    typedef itk::ConstantPadImageFilter< InputImageType, FilterImageType > ConstantPadType;
    ConstantPadType::Pointer constantPad = ConstantPadType::New();
    constantPad->SetPadLowerBound(lowerExtendRegion);
    constantPad->SetPadUpperBound(upperExtendRegion);
    constantPad->SetConstant( zeroPixel );
    constantPad->SetInput(reader->GetOutput());
    constantPad->Update();

    // Create and the filter
    typedef itk::TensorFlowImageFilter<FilterImageType, OutputImageType>  FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(constantPad->GetOutput());
    filter->SetModelPath("../../tests/testData/model2.pb");
    filter->SetInputNodeName("ImageTensor");
    filter->SetOutputNodeName("SemanticPredictions");
    filter->SetInputTensorTF_DataType(TF_UINT8);
    filter->Update();

    typedef  itk::RescaleIntensityImageFilter< FilterImageType, OutputImageType  > RescaleIntensityType2;
    RescaleIntensityType2::Pointer rescaleIntensity2 = RescaleIntensityType2::New();
    rescaleIntensity2->SetInput(filter->GetOutput());
    rescaleIntensity2->Update();

    auto pos = outputFilename.rfind(fileSeparator());
    if (pos!= std::string::npos) {
        itk::FileTools::CreateDirectory(outputFilename.substr(0, pos));
    }

    typedef  itk::ImageFileWriter< OutputImageType  > WriterType;
    WriterType::Pointer writer = WriterType::New();
    //writer->SetImageIO( gdcmImageIO );
    writer->SetFileName(outputFilename);
    writer->SetInput(rescaleIntensity2->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;

}

#endif //ITKTENSORFLOW_SAMPLEPIPELINE_H
