//
// Created by Konrad Werys on 01/04/2019.
//

#include "gtest/gtest.h"
#include "tensorToItkImage.h"

TEST(tensorToItkImage, tensorToItkImage2d_test) {

    long sizeX = 256, sizeY = 128;
    const std::vector<std::int64_t> input_dims = {1, sizeX, sizeY};
    const std::vector<uint8_t> input_vals ((unsigned long)sizeX * sizeY, 1);

    TF_Tensor* inputTensor = tf_utils::CreateTensor(TF_UINT8,
                                                    input_dims.data(), input_dims.size(),
                                                    input_vals.data(), input_vals.size() * sizeof(uint8_t));



    using ImageType = itk::Image< std::uint8_t, 2 >;
    ImageType::Pointer image = ImageType::New();

    EXPECT_NO_THROW(tensorToItkImage<ImageType>(inputTensor, image));

    TF_DeleteTensor(inputTensor);

}

TEST(tensorToItkImage, tensorToItkImage3d_test) {

    long sizeX = 128, sizeY = 256, sizeZ = 3;
    const std::vector<std::int64_t> input_dims = {1, sizeX, sizeY, sizeZ};
    const std::vector<float> input_vals ((unsigned long)sizeX * sizeY * sizeZ, 1);

    TF_Tensor* inputTensor = tf_utils::CreateTensor(TF_FLOAT,
                                                    input_dims.data(), input_dims.size(),
                                                    input_vals.data(), input_vals.size() * sizeof(float));

    using ImageType = itk::Image< std::float_t, 3 >;
    ImageType::Pointer image = ImageType::New();

    EXPECT_NO_THROW(tensorToItkImage<ImageType>(inputTensor, image));

    TF_DeleteTensor(inputTensor);

}