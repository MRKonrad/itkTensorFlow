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
int tensorToItkImage(const TF_Tensor* inputTensor, ImageType* outputImage) {

    typedef typename ImageType::PixelType PixelType;

    TF_DataType dataType = TF_TensorType(inputTensor);

    if (dataType == TF_FLOAT && typeid(PixelType) != typeid(std::float_t)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_FLOAT) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_DOUBLE && typeid(PixelType) != typeid(std::double_t)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_DOUBLE) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_INT32 && typeid(PixelType) != typeid(std::int32_t)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_INT32) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_UINT8 && typeid(PixelType) != typeid(std::uint8_t)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_UINT8) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_INT16 && typeid(PixelType) != typeid(std::int16_t)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_INT16) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_INT8 && typeid(PixelType) != typeid(std::int8_t)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_INT8) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_COMPLEX64 && typeid(PixelType) != typeid(std::complex<std::float_t>)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_COMPLEX64) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_INT64 && typeid(PixelType) != typeid(std::int64_t)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_INT64) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_UINT16 && typeid(PixelType) != typeid(std::uint16_t)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_UINT16) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_COMPLEX128 && typeid(PixelType) != typeid(std::complex<std::double_t>)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_COMPLEX128) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_UINT32 && typeid(PixelType) != typeid(std::uint32_t)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_UINT32) << std::endl;
        return 1; // EXIT_FAILURE
    }
    if (dataType == TF_UINT64 && typeid(PixelType) != typeid(std::uint64_t)) {
        std::cout << "Output image pixel type not compatible with " << TFDataTypeToString(TF_UINT64) << std::endl;
        return 1; // EXIT_FAILURE
    }

    if (TF_DataTypeSize(dataType) != sizeof(PixelType)) {
        std::cout << "Size of TF_DataType " << TFDataTypeToString(dataType) << ": " << TF_DataTypeSize(dataType)
                  << " differs from size of PixelType: " << sizeof(PixelType) << std::endl;
        return 1; // EXIT_FAILURE
    }

    typename ImageType::IndexType start;
    start.Fill(0);

    auto tensor_num_dims = TF_NumDims(inputTensor);
    auto image_num_dims = start.GetIndexDimension() + 1;

    if (tensor_num_dims != image_num_dims) {
        std::cout << "Output image num_dims: " << image_num_dims << " Tensor num_dims: " << tensor_num_dims
                  << std::endl;
        return 1; // EXIT_FAILURE
    }

    typename ImageType::SizeType size_temp;
    for (int i = 1; i < tensor_num_dims; i++) {
        size_temp[i - 1] = TF_Dim(inputTensor, i);
    }

    typename ImageType::RegionType region_temp;
    region_temp.SetSize(size_temp);
    region_temp.SetIndex(start);

    outputImage->SetRegions(region_temp);
    outputImage->Allocate();

    const auto tensor_data = static_cast<PixelType *>(TF_TensorData(inputTensor));

    for (int i = 0; i < size_temp[0] * size_temp[1]; ++i) {
        if (image_num_dims == 3) {
            outputImage->GetBufferPointer()[i] = tensor_data[i];
        }
        else if (image_num_dims == 4) {

            for (int j = 0; j < size_temp[2]; j++) {
                outputImage->GetBufferPointer()[i + j * size_temp[0] * size_temp[1]] = tensor_data[i * size_temp[2] + j];
            }
        }
    }

    return 0; //EXIT_SUCCESS
}

#endif //ITKTENSORFLOW_tensorToItkImage_H
