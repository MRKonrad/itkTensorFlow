//
// Created by Konrad Werys on 06/04/2019.
//

#ifndef ITKTENSORFLOW_OXTFIMAGETOTENSOR_HXX
#define ITKTENSORFLOW_OXTFIMAGETOTENSOR_HXX

namespace oxtf{

    template< typename ImageType>
    int
    ImageToTensor<ImageType>
    ::convertWithCasting(const ImageType *inputImage, TF_DataType dataType, TF_Tensor **outputTensor) {

        typedef typename ImageType::PixelType PixelType;

        // first get info about the image
        auto nPixels = inputImage->GetLargestPossibleRegion().GetNumberOfPixels();
        auto image_num_dims = inputImage->GetLargestPossibleRegion().GetSize().GetSizeDimension();
        std::size_t tensor_num_dims = 4;

        if (image_num_dims > 3) {
            std::cerr << "ImageToTensor not ready for more than 4 dims" << std::endl;
            return 1; // EXIT_FAILURE
        }

        std::vector<std::int64_t> dims(4, 1);
        dims[1] = inputImage->GetLargestPossibleRegion().GetSize()[1];
        dims[2] = inputImage->GetLargestPossibleRegion().GetSize()[0];
        if (image_num_dims > 2) {
            dims[3] = inputImage->GetLargestPossibleRegion().GetSize()[2];
        }

        if (dataType == TF_FLOAT) {
            createTensor<PixelType, std::float_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels,
                                                  dataType, tensor_num_dims);
        } else if (dataType == TF_DOUBLE) {
            createTensor<PixelType, std::double_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels,
                                                   dataType, tensor_num_dims);
        } else if (dataType == TF_INT32) {
            createTensor<PixelType, std::int32_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels,
                                                  dataType, tensor_num_dims);
        } else if (dataType == TF_UINT8) {
            createTensor<PixelType, std::uint8_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels,
                                                  dataType, tensor_num_dims);
        } else if (dataType == TF_INT16) {
            createTensor<PixelType, std::int16_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels,
                                                  dataType, tensor_num_dims);
        } else if (dataType == TF_INT8) {
            createTensor<PixelType, std::int8_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels,
                                                 dataType, tensor_num_dims);
        } else if (dataType == TF_COMPLEX64) {
            createTensor<PixelType, std::complex<std::float_t> >(outputTensor, dims, inputImage->GetBufferPointer(),
                                                                 nPixels, dataType, tensor_num_dims);
        } else if (dataType == TF_INT64) {
            createTensor<PixelType, std::int64_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels,
                                                  dataType, tensor_num_dims);
        } else if (dataType == TF_UINT16) {
            createTensor<PixelType, std::uint16_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels,
                                                   dataType, tensor_num_dims);
        } else if (dataType == TF_COMPLEX128) {
            createTensor<PixelType, std::complex<std::double_t>>(outputTensor, dims, inputImage->GetBufferPointer(),
                                                                 nPixels, dataType, tensor_num_dims);
        } else if (dataType == TF_UINT32) {
            createTensor<PixelType, std::uint32_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels,
                                                   dataType, tensor_num_dims);
        } else if (dataType == TF_UINT64) {
            createTensor<PixelType, std::uint64_t>(outputTensor, dims, inputImage->GetBufferPointer(), nPixels,
                                                   dataType, tensor_num_dims);
        } else {
            std::cerr << "ImageToTensor::convert incompatible data type" << std::endl;
            return 1; // EXIT_FAILURE
        }

        return 0; // EXIT_SUCCESS
    }

    template< typename ImageType>
    int
    ImageToTensor<ImageType>
    ::convert(const ImageType *inputImage, TF_Tensor **outputTensor) {

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
            std::cerr << "ImageToTensor::convert incompatible data type" << std::endl;
            return 1; // EXIT_FAILURE
        }

        convertWithCasting(inputImage, dataType, outputTensor);
        return 0; //EXIT_SUCCESS
    }

    template< typename ImageType>
    template<typename ImagePixelType, typename TensorPixelType>
    int
    ImageToTensor<ImageType>
    ::convertImageBufferToTensorBuffer(std::vector<std::int64_t> dims, ImagePixelType *pixelBuffer,
                                     TensorPixelType *tensorBuffer) {
        for (int x = 0; x < dims[1]; x++) {
            for (int y = 0; y < dims[2]; y++) {
                for (int z = 0; z < dims[3]; z++) {
                    tensorBuffer[y * dims[1] * dims[3] + x * dims[3] + z] =
                            static_cast<TensorPixelType>(pixelBuffer[x + y * dims[1] + z * dims[1] * dims[2]]);
//                    tensorBuffer[ x * dims[2] * dims[3] + y * dims[3] + z ] =
//                            static_cast<TensorPixelType>(pixelBuffer[ x + y * dims[1] + z * dims[1] * dims[2] ]);
                }
            }
        }
        return 0; //EXIT_SUCCESS
    };

    template< typename ImageType>
    template<typename ImagePixelType, typename TensorPixelType>
    int
    ImageToTensor<ImageType>
    ::createTensor(TF_Tensor **outputTensor, std::vector<std::int64_t> dims, const ImagePixelType *pixelBuffer,
                   unsigned long nPixels, TF_DataType dataType, size_t num_dims) {

        std::vector<TensorPixelType> input_vals(nPixels, 0);
        convertImageBufferToTensorBuffer(dims, pixelBuffer, input_vals.data());
        *outputTensor = tf_utils::CreateTensor(dataType, dims.data(), num_dims, input_vals.data(),
                                               nPixels * TF_DataTypeSize(dataType));

        return 0; // EXIT_SUCCESS
    }
}
#endif //ITKTENSORFLOW_OXTFIMAGETOTENSOR_HXX
