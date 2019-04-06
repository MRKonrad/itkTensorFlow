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


    template <typename TensorPixelType, typename ImagePixelType>
    int
    convertTensorBufferToImageBuffer(std::vector<std::int64_t> dims, TensorPixelType* tensorBuffer, ImagePixelType* pixelBuffer){
        for (int x = 0; x < dims[1]; x++){
            for (int y = 0; y < dims[2]; y++){
                for (int z = 0; z < dims[3]; z++){
                    pixelBuffer[x + y * dims[1] + z * dims[1] * dims[2]] =
                            static_cast<TensorPixelType>(tensorBuffer[ y * dims[1] * dims[3] + x * dims[3] + z ]);
//                    pixelBuffer[x + y * dims[1] + z * dims[1] * dims[2]] =
//                            static_cast<TensorPixelType>(tensorBuffer[ x * dims[2] * dims[3] + y * dims[3] + z ]);
                }
            }
        }

        return 0; //EXIT_SUCCESS
    };

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

        typename ImageType::IndexType start;
        start.Fill(0);

        auto tensor_num_dims = TF_NumDims(inputTensor);
        auto image_num_dims = start.GetIndexDimension();

        std::vector<std::int64_t> dims(4, 1);
        dims[1] = TF_Dim(inputTensor, 2);
        dims[2] = TF_Dim(inputTensor, 1);
        if (tensor_num_dims > 3) {
            dims[3] = TF_Dim(inputTensor, 3);
        }

        typename ImageType::SizeType size;
        size.Fill(1);
        size[0] = dims[1];
        size[1] = dims[2];
        if (image_num_dims > 2) {
            size[2] = dims[3];
        }

        typename ImageType::RegionType region_temp;
        region_temp.SetSize(size);
        region_temp.SetIndex(start);

        outputImage->SetRegions(region_temp);
        outputImage->Allocate();
        
        TF_DataType dataType = TF_TensorType(inputTensor);

        if (dataType == TF_FLOAT) {
            const auto tensor_data = static_cast<std::float_t *>(TF_TensorData(inputTensor));
            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_DOUBLE) {
            const auto tensor_data = static_cast<std::double_t *>(TF_TensorData(inputTensor));
            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_INT32) {
            const auto tensor_data = static_cast<std::int32_t *>(TF_TensorData(inputTensor));
            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_UINT8) {
            const auto tensor_data = static_cast<std::uint8_t *>(TF_TensorData(inputTensor));
            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_INT16) {
            const auto tensor_data = static_cast<std::int16_t *>(TF_TensorData(inputTensor));
            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_INT8) {
            const auto tensor_data = static_cast<std::int8_t *>(TF_TensorData(inputTensor));
            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_COMPLEX64) {
            std::cerr << "TF_COMPLEX64 output type not implemented" << std::endl;
            return 1; //EXIT_FAILURE
//            const auto tensor_data = static_cast<std::complex<std::float_t> *>(TF_TensorData(inputTensor));
//            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_INT64) {
            const auto tensor_data = static_cast<std::int64_t *>(TF_TensorData(inputTensor));
            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_UINT16) {
            const auto tensor_data = static_cast<std::uint16_t *>(TF_TensorData(inputTensor));
            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_COMPLEX128) {
            std::cerr << "TF_COMPLEX128 output type not implemented" << std::endl;
            return 1; //EXIT_FAILURE
//            const auto tensor_data = static_cast<std::complex<std::double_t> *>(TF_TensorData(inputTensor));
//            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_UINT32) {
            const auto tensor_data = static_cast<std::uint32_t *>(TF_TensorData(inputTensor));
            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else if (dataType == TF_UINT64) {
            const auto tensor_data = static_cast<std::uint64_t *>(TF_TensorData(inputTensor));
            convertTensorBufferToImageBuffer(dims, tensor_data, outputImage->GetBufferPointer());
        } else {
            std::cerr << "Given tensor output type not implemented" << std::endl;
            return 1; //EXIT_FAILURE
        }

        return 0; //EXIT_SUCCESS
    }
} // namespace oxtf

#endif //ITKTENSORFLOW_tensorToItkImage_H
