//
// Created by Konrad Werys on 01/04/2019.
//

#ifndef ITKTENSORFLOW_tensorToItkImage_H
#define ITKTENSORFLOW_tensorToItkImage_H

#include "itkPermuteAxesImageFilter.h"
#include <c_api.h>
#include "tf_utils.hpp"
#include "oxtfUtils.h"

// TODO: make a class

namespace oxtf {

/**
 *
 * @tparam ImageType
 * @param inputTensor
 * @param outputImage - non allocated itk image
 * @return
 */
    template<typename ImageType>
    int TensorToImage(const TF_Tensor *inputTensor, ImageType *outputImage) {

        typedef typename ImageType::PixelType PixelType;

        if (inputTensor == nullptr) {
            std::cerr << "TensorToImage: empty input tensor" << std::endl;
            return 1; // EXIT_FAILURE
        }

        TF_DataType dataType = TF_TensorType(inputTensor);

        if (dataType == TF_FLOAT && typeid(PixelType) != typeid(std::float_t)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with " << TFDataTypeToString(TF_FLOAT)
                      << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_DOUBLE && typeid(PixelType) != typeid(std::double_t)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with " << TFDataTypeToString(TF_DOUBLE)
                      << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_INT32 && typeid(PixelType) != typeid(std::int32_t)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with " << TFDataTypeToString(TF_INT32)
                      << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_UINT8 && typeid(PixelType) != typeid(std::uint8_t)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with " << TFDataTypeToString(TF_UINT8)
                      << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_INT16 && typeid(PixelType) != typeid(std::int16_t)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with " << TFDataTypeToString(TF_INT16)
                      << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_INT8 && typeid(PixelType) != typeid(std::int8_t)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with " << TFDataTypeToString(TF_INT8)
                      << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_COMPLEX64 && typeid(PixelType) != typeid(std::complex<std::float_t>)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with "
                      << TFDataTypeToString(TF_COMPLEX64) << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_INT64 && typeid(PixelType) != typeid(std::int64_t)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with " << TFDataTypeToString(TF_INT64)
                      << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_UINT16 && typeid(PixelType) != typeid(std::uint16_t)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with " << TFDataTypeToString(TF_UINT16)
                      << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_COMPLEX128 && typeid(PixelType) != typeid(std::complex<std::double_t>)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with "
                      << TFDataTypeToString(TF_COMPLEX128) << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_UINT32 && typeid(PixelType) != typeid(std::uint32_t)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with " << TFDataTypeToString(TF_UINT32)
                      << std::endl;
            return 1; // EXIT_FAILURE
        }
        if (dataType == TF_UINT64 && typeid(PixelType) != typeid(std::uint64_t)) {
            std::cerr << "TensorToImage: output image pixel type not compatible with " << TFDataTypeToString(TF_UINT64)
                      << std::endl;
            return 1; // EXIT_FAILURE
        }

        if (TF_DataTypeSize(dataType) != sizeof(PixelType)) {
            std::cerr << "Size of TF_DataType " << TFDataTypeToString(dataType) << ": " << TF_DataTypeSize(dataType)
                      << " differs from size of PixelType: " << sizeof(PixelType) << std::endl;
            return 1; // EXIT_FAILURE
        }

        typename ImageType::IndexType start;
        start.Fill(0);

        auto tensor_num_dims = TF_NumDims(inputTensor);
        auto image_num_dims = start.GetIndexDimension();

        if (tensor_num_dims - 1 != image_num_dims) {
            std::cerr << "TensorToImage: output image num_dims: " << image_num_dims << " tensor num_dims: "
                      << tensor_num_dims  << " (tensor should have 1 more dim) " << std::endl;
            return 1; // EXIT_FAILURE
        }

        std::vector<std::int64_t> dims(4, 1);
        typename ImageType::SizeType size;
        for (int i = 1; i < tensor_num_dims; i++) {
            size[i - 1] = TF_Dim(inputTensor, i);
            dims[i] = TF_Dim(inputTensor, i);
        }

        typename ImageType::RegionType region_temp;
        region_temp.SetSize(size);
        region_temp.SetIndex(start);

        outputImage->SetRegions(region_temp);
        outputImage->Allocate();

        const auto tensor_data = static_cast<PixelType *>(TF_TensorData(inputTensor));

        for (int x = 0; x < dims[1]; x++){
            for (int y = 0; y < dims[2]; y++){
                for (int z = 0; z < dims[3]; z++){
                    outputImage->GetBufferPointer()[x + y * dims[1] + z * dims[1] * dims[2]] =
                            tensor_data[ y * dims[1] * dims[3] + x * dims[3] + z ];
                }
            }
        }

        return 0; //EXIT_SUCCESS
    }
} // namespace oxtf

#endif //ITKTENSORFLOW_tensorToItkImage_H
