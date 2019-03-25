//
// Created by Konrad Werys on 25/03/2019.
//

#ifndef ITKTENSORFLOW_SAMPLEPIPELINE_H
#define ITKTENSORFLOW_SAMPLEPIPELINE_H

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkFileTools.h"
#include "itkGDCMImageIO.h"
#include "itkTensorFlowFilter.h"

int samplePipeline(int argc, char *argv[]){

    if(argc < 3)
    {
        std::cerr << "Required: input filename and output filename" << std::endl;

        return EXIT_FAILURE;
    }
    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];

    typedef itk::Image< double, 2 >         InputImageType;
    typedef itk::Image< int , 2 >       OutputImageType;
    typedef itk::ImageFileReader<InputImageType> ReaderType;

    typedef itk::GDCMImageIO           ImageIOType;
    ImageIOType::Pointer gdcmImageIO = ImageIOType::New();


    ReaderType::Pointer reader = ReaderType::New();
//    reader->SetImageIO( gdcmImageIO );
    reader->SetFileName(argv[1]);

    typedef itk::TensorFlowImageFilter<InputImageType>  FilterType;

    // Create and the filter
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(reader->GetOutput());
    filter->Update();

    itk::Index<2> cornerPixel = reader->GetOutput()->GetLargestPossibleRegion().GetIndex();

    // The output here is:
    // 0
    // 3
    // That is, the filter changed the pixel, but the input remained unchagned.
    std::cout << reader->GetOutput()->GetPixel(cornerPixel) << std::endl;
    std::cout << filter->GetOutput()->GetPixel(cornerPixel) << std::endl;

    typedef  itk::CastImageFilter< InputImageType, OutputImageType  > CasterType;
    CasterType::Pointer caster = CasterType::New();
    caster->SetInput(filter->GetOutput());
    caster->Update();

    auto pos = outputFilename.rfind(fileSeparator());
    if (pos!= std::string::npos) {
        itk::FileTools::CreateDirectory(outputFilename.substr(0, pos));
    }

    typedef  itk::ImageFileWriter< OutputImageType  > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetImageIO( gdcmImageIO );
    writer->SetFileName(outputFilename);
    writer->SetInput(caster->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;

}

#endif //ITKTENSORFLOW_SAMPLEPIPELINE_H
