//
// Created by Konrad Werys on 01/04/2019.
//

#ifndef ITKTENSORFLOW_tensorToItkImage_H
#define ITKTENSORFLOW_tensorToItkImage_H

#include "itkPermuteAxesImageFilter.h"
#include <c_api.h>
#include "tf_utils.hpp"
#include "itktfUtils.h"

/**
 *
 * @tparam ImageType
 * @param inputTensor
 * @param outputImage - non allocated itk image
 * @return
 */
template <typename ImageType>
int tensorToItkImage(const TF_Tensor* inputTensor, ImageType* outputImage){

    typedef typename ImageType::PixelType PixelType;

    TF_DataType dataType = TF_TensorType(inputTensor);

    if ( dataType == TF_FLOAT && typeid(PixelType) != typeid(std::float_t) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_FLOAT) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_DOUBLE && typeid(PixelType) != typeid(std::double_t) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_DOUBLE) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_INT32 && typeid(PixelType) != typeid(std::int32_t) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_INT32) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_UINT8 && typeid(PixelType) != typeid(std::uint8_t) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_UINT8) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_INT16 && typeid(PixelType) != typeid(std::int16_t) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_INT16) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_INT8 && typeid(PixelType) != typeid(std::int8_t) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_INT8) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_COMPLEX64 && typeid(PixelType) != typeid(std::complex<std::float_t>) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_COMPLEX64) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_INT64 && typeid(PixelType) != typeid(std::complex<std::int64_t>) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_INT64) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_UINT16 && typeid(PixelType) != typeid(std::complex<std::uint16_t>) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_UINT16) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_COMPLEX128 && typeid(PixelType) != typeid(std::complex<std::double_t>) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_COMPLEX128) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_COMPLEX128 && typeid(PixelType) != typeid(std::complex<std::uint32_t>) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_COMPLEX128) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if ( dataType == TF_UINT64 && typeid(PixelType) != typeid(std::complex<std::uint64_t>) ){
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_UINT64) << std::endl;
        return 1; // EXIT_FAILURE
    }

    if (TF_DataTypeSize(dataType) != sizeof(PixelType)){
        std::cout << "Size of TF_DataType " << TFDataTypeToString(dataType) << ": " << TF_DataTypeSize(dataType)
                  << " differs from size of PixelType: " << sizeof(PixelType) << std::endl;
        return 1; // EXIT_FAILURE
    }

    typename ImageType::IndexType start;
    start.Fill(0);

    auto tensor_num_dims = TF_NumDims(inputTensor);
    auto image_num_dims = start.GetIndexDimension() + 1;

    if (tensor_num_dims != image_num_dims){
        std::cout << "Output image num_dims: " << image_num_dims << " Tensor num_dims: " << tensor_num_dims << std::endl;
        return 1; // EXIT_FAILURE
    }

    typename ImageType::SizeType  size_temp;
    typename ImageType::SizeType  size_output;
    for (int i = 1; i < tensor_num_dims; i++){
        size_temp[i-1] = TF_Dim(inputTensor, tensor_num_dims-i);
        size_output[i-1] = TF_Dim(inputTensor, i);
    }

    typename ImageType::RegionType region_output;
    region_output.SetSize( size_output );
    region_output.SetIndex( start );

    outputImage->SetRegions( region_output );
    outputImage->Allocate();

    typename ImageType::RegionType region_temp;
    region_temp.SetSize( size_temp );
    region_temp.SetIndex( start );

    typename ImageType::Pointer tempImage = ImageType::New();
    tempImage->SetRegions( region_temp );
    tempImage->Allocate();

    void* tensor_data = TF_TensorData(inputTensor);
    std::memcpy(tempImage->GetBufferPointer(), tensor_data, TF_TensorByteSize(inputTensor));

    typedef itk::PermuteAxesImageFilter<ImageType> PermuteFilterType;
    typename PermuteFilterType::PermuteOrderArrayType order;
    for (int i = 0; i < tensor_num_dims-1; i++){
        order[i] = tensor_num_dims-2-i;
    }
    typename PermuteFilterType::Pointer permuteFilter = PermuteFilterType::New();
    permuteFilter->SetInput(tempImage);
    permuteFilter->Update();

    // TODO: find a more elegant way
    std::memcpy(outputImage->GetBufferPointer(), permuteFilter->GetOutput()->GetBufferPointer(), TF_TensorByteSize(inputTensor));

    return 0; //EXIT_SUCCESS
}

#endif //ITKTENSORFLOW_tensorToItkImage_H
