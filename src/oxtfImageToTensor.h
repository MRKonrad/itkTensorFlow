//
// Created by Konrad Werys on 01/04/2019.
//

#ifndef ITKTENSORFLOW_ITKIMAGETOTENSOR_H
#define ITKTENSORFLOW_ITKIMAGETOTENSOR_H

#include "itkImage.h"
#include <c_api.h>
#include "tf_utils.hpp"

namespace oxtf {

    template< typename ImageType>
    class ImageToTensor {

    public:
        ImageToTensor() = default;
        virtual ~ImageToTensor() = default;

        /**
        * Create a new tensor by copying the buffed data from the image.
        * Consider changing it to a class
        * @tparam ImageType
        * @param inputImage
        * @param outputTensor
        * @return
        */
        static
        int
        convertWithCasting(const ImageType *inputImage, TF_DataType dataType, TF_Tensor **outputTensor);

        /**
         * deducing TF_DataType from the inputImage PixelType to run ImageToTensor::convertWithCasting
         * @tparam ImageType
         * @param inputImage
         * @param outputTensor
         * @return
         */
        static
        int
        convert(const ImageType *inputImage, TF_Tensor **outputTensor);

        /**
         * Coping one buffer to the other. The order of pixels have been chosen experimentally
         * @tparam ImagePixelType
         * @tparam TensorPixelType
         * @param dims
         * @param pixelBuffer
         * @param tensorBuffer
         * @return
         */
        template<typename ImagePixelType, typename TensorPixelType>
        static
        int
        convertImageBufferToTensorBuffer(std::vector<std::int64_t> dims, ImagePixelType *pixelBuffer,
                                         TensorPixelType *tensorBuffer);

        /**
         * A convenience function to be used in the big switch with different types
         * @tparam ImagePixelType
         * @tparam TensorPixelType
         * @param outputTensor
         * @param dims
         * @param pixelBuffer
         * @param nPixels
         * @param dataType
         * @param num_dims
         * @return
         */
        template<typename ImagePixelType, typename TensorPixelType>
        static
        int createTensor(TF_Tensor **outputTensor, std::vector<std::int64_t> dims, const ImagePixelType *pixelBuffer,
                         unsigned long nPixels, TF_DataType dataType, size_t num_dims);
    };

 } //namespace oxtf

#include "oxtfImageToTensor.hxx"

#endif //ITKTENSORFLOW_ITKIMAGETOTENSOR_H
