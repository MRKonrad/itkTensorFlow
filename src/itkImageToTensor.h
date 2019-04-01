//
// Created by Konrad Werys on 01/04/2019.
//

#ifndef ITKTENSORFLOW_ITKIMAGETOTENSOR_H
#define ITKTENSORFLOW_ITKIMAGETOTENSOR_H

#include "itkPermuteAxesImageFilter.h"
#include <c_api.h>
#include "tf_utils.hpp"



template <typename ImageType>
int itkImageToTensor(const typename ImageType::Pointer inputImage, TF_Tensor** outputTensor){

    typedef typename ImageType::PixelType PixelType;

    TF_DataType dataType;

    if        ( typeid(PixelType) == typeid(std::float_t) ){
        dataType = TF_FLOAT;
    } else if ( typeid(PixelType) == typeid(std::double_t) ){
        dataType = TF_DOUBLE;
    } else if ( typeid(PixelType) == typeid(std::int32_t) ){
        dataType = TF_INT32;
    } else if ( typeid(PixelType) == typeid(std::uint8_t) ){
        dataType = TF_UINT8;
    } else if ( typeid(PixelType) == typeid(std::int16_t) ){
        dataType = TF_INT16;
    } else if ( typeid(PixelType) == typeid(std::int8_t) ){
        dataType = TF_INT8;
    } else if ( typeid(PixelType) == typeid(std::complex<std::float_t>) ){
        dataType = TF_COMPLEX64;
    } else if ( typeid(PixelType) == typeid(std::int64_t) ){
        dataType = TF_INT64;
    } else if ( typeid(PixelType) == typeid(std::uint16_t) ){
        dataType = TF_UINT16;
    } else if ( typeid(PixelType) == typeid(std::complex<std::double_t>) ){
        dataType = TF_COMPLEX128;
    } else if ( typeid(PixelType) == typeid(std::uint32_t) ){
        dataType = TF_UINT32;
    } else if ( typeid(PixelType) == typeid(std::uint64_t) ){
        dataType = TF_UINT64;
    } else {
        return 1; // EXIT_FAILURE
    }

    // first get info about the image
    auto nPixels = inputImage->GetLargestPossibleRegion().GetNumberOfPixels();
    auto num_dims = inputImage->GetLargestPossibleRegion().GetSize().GetSizeDimension() + 1;
    auto dims = new std::int64_t[num_dims];
    dims[0] = 1;
    for (int i = 1; i < num_dims; i++){
        dims[i] = inputImage->GetLargestPossibleRegion().GetSize()[i-1];
    }

    // then convert the axes
    typedef itk::PermuteAxesImageFilter<ImageType> PermuteFilterType;
    typename PermuteFilterType::PermuteOrderArrayType order;
    for (int i = 0; i < num_dims-1; i++){
        order[i] = num_dims-2-i;
    }
    typename PermuteFilterType::Pointer permuteFilter = PermuteFilterType::New();
    permuteFilter->SetInput(inputImage);
    permuteFilter->Update();

//    *outputTensor = tf_utils::CreateTensor(dataType,
//                                          dims,
//                                          num_dims,
//                                          inputImage->GetBufferPointer(),
//                                          nPixels * TF_DataTypeSize(dataType));

    *outputTensor = tf_utils::CreateTensor(dataType,
                                           dims,
                                           num_dims,
                                           permuteFilter->GetOutput()->GetBufferPointer(),
                                           nPixels * TF_DataTypeSize(dataType));

    delete [] dims;

    return 0; //EXIT_SUCCESS
}

#endif //ITKTENSORFLOW_ITKIMAGETOTENSOR_H
