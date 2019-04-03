//
// Created by Konrad Werys on 01/04/2019.
//

#include "gtest/gtest.h"

#include "oxtfImageToTensor.h"


TEST(ImageToTensor, ImageToTensor2d_test) {

    using ImageType = itk::Image< std::uint64_t, 2 >;
    ImageType::Pointer image = ImageType::New();

    ImageType::IndexType start;
    start[0] = 0;  // first index on X
    start[1] = 0;  // first index on Y

    ImageType::SizeType  size;
    size[0] = 100;  // size along X
    size[1] = 200;  // size along Y

    ImageType::RegionType region;
    region.SetSize( size );
    region.SetIndex( start );

    image->SetRegions( region );
    image->Allocate();

    TF_Tensor* tensor;

    EXPECT_NO_THROW(oxtf::ImageToTensor<ImageType>(image, &tensor));

    TF_DeleteTensor(tensor);

}

TEST(ImageToTensor, ImageToTensor3d_test) {

    using ImageType = itk::Image< unsigned char, 3 >;
    ImageType::Pointer image = ImageType::New();

    ImageType::IndexType start;
    start[0] = 0;  // first index on X
    start[1] = 0;  // first index on Y
    start[2] = 0;  // first index on Z

    ImageType::SizeType  size;
    size[0] = 100;  // size along X
    size[1] = 200;  // size along Y
    size[2] = 300;  // size along Z

    ImageType::RegionType region;
    region.SetSize( size );
    region.SetIndex( start );

    image->SetRegions( region );
    image->Allocate();

    TF_Tensor* tensor;

    EXPECT_NO_THROW(oxtf::ImageToTensor<ImageType>(image, &tensor));

    TF_DeleteTensor(tensor);

}