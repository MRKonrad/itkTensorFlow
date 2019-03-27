//
// Created by Konrad Werys on 25/03/2019.
//

#ifndef ITKTENSORFLOW_SAMPLEPIPELINE2_H
#define ITKTENSORFLOW_SAMPLEPIPELINE2_H

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

int samplePipeline1(int argc, char **argv){

//    if(argc < 3)
//    {
//        std::cerr << "Required: input filename and output filename" << std::endl;
//
//        return EXIT_FAILURE;
//    }
//    std::string inputFilename = argv[1];
//    std::string outputFilename = argv[2];
//
//    typedef itk::Image< float, 2 > InputImageType;
//    typedef itk::Image< float, 2 > FilterImageType;
//    typedef itk::Image< int, 2 > OutputImageType;
//    typedef itk::ImageFileReader<InputImageType> ReaderType;
//
//    typedef itk::GDCMImageIO           ImageIOType;
//    ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
//
//    ReaderType::Pointer reader = ReaderType::New();
////    reader->SetImageIO( gdcmImageIO );
//    reader->SetFileName(argv[1]);
//    reader->Update();
//
//    typedef  itk::MinimumMaximumImageCalculator<InputImageType> MinimumMaximumImageCalculatorType;
//    MinimumMaximumImageCalculatorType::Pointer mmFiler = MinimumMaximumImageCalculatorType::New();
//    mmFiler->SetImage(reader->GetOutput());
//    mmFiler->Compute();
//    std::cout << mmFiler->GetMinimum() << " " <<  mmFiler->GetMaximum() << std::endl;
//
////    typedef  itk::RescaleIntensityImageFilter< InputImageType, FilterImageType  > RescaleIntensityType;
////    RescaleIntensityType::Pointer rescaleIntensity = RescaleIntensityType::New();
////    rescaleIntensity->SetInput(reader->GetOutput());
////    rescaleIntensity->Update();
////
////    typedef  itk::MinimumMaximumImageCalculator<FilterImageType> MinimumMaximumImageCalculatorType2;
////    MinimumMaximumImageCalculatorType2::Pointer mmFiler2 = MinimumMaximumImageCalculatorType2::New();
////    mmFiler2->SetImage(rescaleIntensity->GetOutput());
////    mmFiler2->Compute();
////    std::cout << mmFiler2->GetMinimum() << " " <<  mmFiler2->GetMaximum() << std::endl;
//
////    typedef itk::ShiftScaleImageFilter< InputImageType, FilterImageType  > ShiftScaleType;
////    ShiftScaleType::Pointer shiftScale = ShiftScaleType::New();
////    shiftScale->SetInput(reader->GetOutput());
////    //shiftScale->SetScale(0.0625);
////    shiftScale->SetScale(10);
////    shiftScale->Update();
////
//    using RegionType = itk::ImageRegion< 2 >;
//    RegionType region = reader->GetOutput()->GetLargestPossibleRegion();
//
//    FilterImageType::SizeType lowerExtendRegion;
//    lowerExtendRegion[0] = 0;
//    lowerExtendRegion[1] = 0;
//
//    FilterImageType::SizeType upperExtendRegion;
//    upperExtendRegion[0] = 384 - region.GetSize()[0];
//    upperExtendRegion[1] = 384 - region.GetSize()[1];
//
//    typedef itk::ConstantPadImageFilter< FilterImageType, FilterImageType > ConstantPadType;
//    ConstantPadType::Pointer constantPad = ConstantPadType::New();
//    constantPad->SetPadLowerBound(lowerExtendRegion);
//    constantPad->SetPadUpperBound(upperExtendRegion);
//    constantPad->SetConstant( 0 );
//    constantPad->SetInput(reader->GetOutput());
//    constantPad->Update();
//
//    // Create and the filter
//    typedef itk::TensorFlowImageFilter<FilterImageType, FilterImageType>  FilterType;
//    FilterType::Pointer filter = FilterType::New();
//    filter->SetInput(constantPad->GetOutput());
//    filter->SetModelPath("../../tests/testData/model.pb");
//    filter->SetInputNodeName("input_1");
//    filter->SetOutputNodeName("conv2d_7/truediv");
//    filter->SetInputTensorTF_DataType(TF_FLOAT);
//    filter->Update();
////
////    typedef  itk::CastImageFilter< FilterImageType, OutputImageType  > CasterType2;
////    CasterType2::Pointer caster2 = CasterType2::New();
////    caster2->SetInput(filter->GetOutput());
////    caster2->Update();
////
////    typedef  itk::RescaleIntensityImageFilter< OutputImageType, OutputImageType  > RescaleIntensityType;
////    RescaleIntensityType::Pointer rescaleIntensity = RescaleIntensityType::New();
////    rescaleIntensity->SetInput(caster2->GetOutput());
////    rescaleIntensity->Update();
////
//
//
//    typedef  itk::RescaleIntensityImageFilter< FilterImageType, OutputImageType  > RescaleIntensityType2;
//    RescaleIntensityType2::Pointer rescaleIntensity2 = RescaleIntensityType2::New();
//    rescaleIntensity2->SetInput(filter->GetOutput());
//    rescaleIntensity2->Update();
//
//    auto pos = outputFilename.rfind(fileSeparator());
//    if (pos!= std::string::npos) {
//        itk::FileTools::CreateDirectory(outputFilename.substr(0, pos));
//    }
//
//    typedef  itk::ImageFileWriter< OutputImageType  > WriterType;
//    WriterType::Pointer writer = WriterType::New();
//    //writer->SetImageIO( gdcmImageIO );
//    writer->SetFileName(outputFilename);
//    writer->SetInput(rescaleIntensity2->GetOutput());
//    writer->Update();

    return EXIT_SUCCESS;

}

#endif //ITKTENSORFLOW_SAMPLEPIPELINE2_H
