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

    /**
     * sadly I do not know how to use itkPermuteAxesImageFilter here
     * @tparam ImageType
     * @param inputImage
     * @param outputTensor
     * @return
     */
     template<typename ImageType>
     int
     ImageToTensor(const ImageType* inputImage, TF_Tensor **outputTensor, unsigned long num_dims = 0) {

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

         // first get info about the image
         auto nPixels = inputImage->GetLargestPossibleRegion().GetNumberOfPixels();
         if (num_dims == 0) {
             num_dims = inputImage->GetLargestPossibleRegion().GetSize().GetSizeDimension() + 1;
         }

         std::vector<std::int64_t> dims(num_dims, 1);
         int image_num_dims = inputImage->GetLargestPossibleRegion().GetIndex().GetIndexDimension();
         for (int i = 0; i < image_num_dims; i++) {
             dims[i + 1] = inputImage->GetLargestPossibleRegion().GetSize()[i];
         }

         std::vector<PixelType> input_vals(nPixels, 0);

         if (num_dims > 4) {
             std::cerr << "ImageToTensor not ready for more than 4 dims" << std::endl;
             return 1; // EXIT_FAILURE
         }

         for (int i = 0; i < dims[1] * dims[2]; ++i) {
             if (num_dims == 3) {
                 input_vals[i] = inputImage->GetBufferPointer()[i];
             } else if (num_dims == 4) {
                 for (int j = 0; j < dims[3]; j++) {
                     input_vals[i * dims[3] + j] = inputImage->GetBufferPointer()[i + j * dims[1] * dims[2]];
                 }
             }
         }

         *outputTensor = tf_utils::CreateTensor(dataType,
                                                dims.data(),
                                                num_dims,
                                                input_vals.data(),
                                                nPixels * TF_DataTypeSize(dataType));

         return 0; //EXIT_SUCCESS
     }
 } //namespace oxtf

#endif //ITKTENSORFLOW_ITKIMAGETOTENSOR_H
