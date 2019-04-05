//
// Created by Konrad Werys on 01/04/2019.
//

#ifndef ITKTENSORFLOW_ITKIMAGETOTENSOR_H
#define ITKTENSORFLOW_ITKIMAGETOTENSOR_H

#include "itkImage.h"
#include <c_api.h>
#include "tf_utils.hpp"

// TODO: make a class

namespace oxtf {


    template <typename ImagePixelType, typename TensorPixelType>
    int
    convertImageBufferToTensorBuffer(std::vector<std::int64_t> dims, ImagePixelType *pixelBuffer,
                                     TensorPixelType *tensorBuffer){
        for (int x = 0; x < dims[1]; x++){
            for (int y = 0; y < dims[2]; y++){
                for (int z = 0; z < dims[3]; z++){
                    tensorBuffer[ y * dims[1] * dims[3] + x * dims[3] + z ] =
                            static_cast<TensorPixelType>(pixelBuffer[ x + y * dims[1] + z * dims[1] * dims[2] ]);
//                    tensorBuffer[ x * dims[2] * dims[3] + y * dims[3] + z ] =
//                            static_cast<TensorPixelType>(pixelBuffer[ x + y * dims[1] + z * dims[1] * dims[2] ]);
                }
            }
        }
        return 0; //EXIT_SUCCESS
    };

    template<typename ImagePixelType, typename TensorPixelType>
    int CreateTensor(TF_Tensor **outputTensor, std::vector<std::int64_t> dims, const ImagePixelType* pixelBuffer,
                     unsigned long nPixels, TF_DataType dataType, size_t num_dims){

        std::vector<TensorPixelType> input_vals(nPixels, 0);
        convertImageBufferToTensorBuffer(dims, pixelBuffer, input_vals.data());
        *outputTensor = tf_utils::CreateTensor(dataType, dims.data(), num_dims, input_vals.data(), nPixels * TF_DataTypeSize(dataType));

        return 0; // EXIT_SUCCESS
    }

    /**
    *
    * @tparam ImageType
    * @param inputImage
    * @param outputTensor
    * @return
    */
    template<typename ImageType>
    int
    ImageToTensorWithCasting(const ImageType* inputImage, TF_DataType dataType, TF_Tensor **outputTensor) {

        typedef typename ImageType::PixelType PixelType;

        // first get info about the image
        auto nPixels = inputImage->GetLargestPossibleRegion().GetNumberOfPixels();
        auto num_dims = inputImage->GetLargestPossibleRegion().GetSize().GetSizeDimension() + 1;

        if (num_dims > 4) {
            std::cerr << "ImageToTensor not ready for more than 4 dims" << std::endl;
            return 1; // EXIT_FAILURE
        }

        std::vector<std::int64_t> dims(4, 1);
        dims[1] = inputImage->GetLargestPossibleRegion().GetSize()[1];
        dims[2] = inputImage->GetLargestPossibleRegion().GetSize()[0];
        if (num_dims > 3) {
            dims[3] = inputImage->GetLargestPossibleRegion().GetSize()[2];
        }

        // TODO: clean this!!!
        num_dims = 4;

        if (dataType == TF_FLOAT) {
            CreateTensor<PixelType, std::float_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_DOUBLE) {
            CreateTensor<PixelType, std::double_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_INT32) {
            CreateTensor<PixelType, std::int32_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_UINT8) {
            CreateTensor<PixelType, std::uint8_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_INT16) {
            CreateTensor<PixelType, std::int16_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_INT8) {
            CreateTensor<PixelType, std::int8_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_COMPLEX64) {
            CreateTensor<PixelType, std::complex<std::float_t> >(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_INT64) {
            CreateTensor<PixelType, std::int64_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_UINT16) {
            CreateTensor<PixelType, std::uint16_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_COMPLEX128) {
            CreateTensor<PixelType, std::complex<std::double_t>>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_UINT32) {
            CreateTensor<PixelType, std::uint32_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else if (dataType == TF_UINT64) {
            CreateTensor<PixelType, std::uint64_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels, dataType, num_dims);
        } else {
            std::cerr << "ImageToTensor: incompatible data type" << std::endl;
            return 1; // EXIT_FAILURE
        }

        return 0; //EXIT_SUCCESS
    }

    /**
     * deducing TF_DataType from the inputImage PixelType to run ImageToTensorWithCasting
     * @tparam ImageType
     * @param inputImage
     * @param outputTensor
     * @return
     */
     template<typename ImageType>
     int
     ImageToTensor(const ImageType* inputImage, TF_Tensor **outputTensor) {

         typedef typename ImageType::PixelType PixelType;

         TF_DataType dataType;

         if (typeid(PixelType) == typeid(std::float_t)) {
             dataType = TF_FLOAT;
         } else if (typeid(PixelType) == typeid(std::double_t)) {
             dataType = TF_DOUBLE;
         } else if (typeid(PixelType) == typeid(std::int32_t)) {
             dataType = TF_INT32;
         } else if (typeid(PixelType) == typeid(std::uint8_t)) {
             dataType = TF_UINT8;
         } else if (typeid(PixelType) == typeid(std::int16_t)) {
             dataType = TF_INT16;
         } else if (typeid(PixelType) == typeid(std::int8_t)) {
             dataType = TF_INT8;
         } else if (typeid(PixelType) == typeid(std::complex<std::float_t>)) {
             dataType = TF_COMPLEX64;
         } else if (typeid(PixelType) == typeid(std::int64_t)) {
             dataType = TF_INT64;
         } else if (typeid(PixelType) == typeid(std::uint16_t)) {
             dataType = TF_UINT16;
         } else if (typeid(PixelType) == typeid(std::complex<std::double_t>)) {
             dataType = TF_COMPLEX128;
         } else if (typeid(PixelType) == typeid(std::uint32_t)) {
             dataType = TF_UINT32;
         } else if (typeid(PixelType) == typeid(std::uint64_t)) {
             dataType = TF_UINT64;
         } else {
             std::cerr << "ImageToTensor: incompatible data type" << std::endl;
             return 1; // EXIT_FAILURE
         }

         ImageToTensorWithCasting(inputImage, dataType, outputTensor);
         return 0; //EXIT_SUCCESS
    }

 } //namespace oxtf

#endif //ITKTENSORFLOW_ITKIMAGETOTENSOR_H
