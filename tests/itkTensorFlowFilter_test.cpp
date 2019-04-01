//
// Created by Konrad Werys on 24/03/2019.
//

#include "gtest/gtest.h"

#include "itktfUtils.h"

#include "samplePipeline2.h"
#include "samplePipeline1.h"

TEST(itkTensorFlow, printTensorFlowVersion_test) {

    EXPECT_NO_THROW(printTensorFlowVersion());

}

TEST(itkTensorFlow, printGraphInfo) {

    EXPECT_NO_THROW(printGraphInfo("../../tests/testData/model2.pb"));

}

//TEST(itkTensorFlow, sessionRun_model) {
//
//    EXPECT_NO_THROW(sessionRun<float>(TF_FLOAT, 384, 384, 1, "../../tests/testData/model.pb", "input_1", "conv2d_7/truediv"));
//
//}

//TEST(itkTensorFlow, sessionRun_model) {
//
//    long sizeX = 384, sizeY=384, sizeZ=1;
//    const std::vector<std::int64_t> input_dims = {1, sizeX, sizeY, sizeZ};
//    const std::vector<float> input_vals (sizeX * sizeY * sizeZ, 1);
//
//    TF_Tensor* input_tensor = tf_utils::CreateTensor(TF_FLOAT,
//                                                     input_dims.data(), input_dims.size(),
//                                                     input_vals.data(), input_vals.size() * sizeof(float));
//    TF_Tensor* output_tensor = nullptr;
//
//    EXPECT_NO_THROW(sessionRun(input_tensor, output_tensor, "../../tests/testData/model.pb", "input_1", "conv2d_7/truediv"));
//
//    const auto data = static_cast<float*>(TF_TensorData(output_tensor));
//
//    std::cout << "Output vals: " << data[0] << ", " << data[1] << ", " << data[2] << ", " << data[3] << std::endl;
//
//    tf_utils::DeleteTensor(input_tensor);
//    tf_utils::DeleteTensor(output_tensor);
//
//}

TEST(itkTensorFlow, sessionRun_model2) {

    long sizeX = 513, sizeY=513, sizeZ=1;
    const std::vector<std::int64_t> input_dims = {1, sizeX, sizeY, sizeZ};
    const std::vector<uint8_t> input_vals (sizeX * sizeY * sizeZ, 1);

    TF_Tensor* input_tensor = tf_utils::CreateTensor(TF_UINT8,
                                                     input_dims.data(), input_dims.size(),
                                                     input_vals.data(), input_vals.size() * sizeof(uint8_t));
    TF_Tensor* output_tensor = nullptr;

    EXPECT_NO_THROW(sessionRun(input_tensor, output_tensor, "../../tests/testData/model2.pb", "ImageTensor", "SemanticPredictions"));

    const auto data = static_cast<float*>(TF_TensorData(output_tensor));

    std::cout << "Output vals: " << data[0] << ", " << data[1] << ", " << data[2] << ", " << data[3] << std::endl;

    tf_utils::DeleteTensor(input_tensor);
    tf_utils::DeleteTensor(output_tensor);

}

TEST(itkTensorFlow, oneImagePipeline1) {

    char *argv[] = {
            (char*) "",
            (char*) "../../tests/testData/dicom/T1map2.dcm",
            (char*) "../../tests/testData/temp/samplePipeline1.dcm",
    };

    samplePipeline1(3, argv);

    EXPECT_EQ(true, true);

}

TEST(itkTensorFlow, oneImagePipeline2) {

    char *argv[] = {
            (char*) "",
            (char*) "../../tests/testData/jpg/image2.jpg",
            (char*) "../../tests/testData/temp/samplePipeline2.png",
    };

    samplePipeline2(3, argv);

    EXPECT_EQ(true, true);

}

TEST(itkTensorFlow, tralala1) {

    char *argv[] = {
            (char*) "",
            (char*) "../../tests/testData/dicom/T1Map.dcm",
            (char*) "../../tests/testData/temp/tralala1.png",
    };

    using PixelType = itk::Vector< uint16_t, 1 >;
    typedef itk::Image< PixelType, 2 > InputImageType;

    typedef itk::ImageFileReader<InputImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();

    typedef  itk::ImageFileWriter< InputImageType  > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(argv[2]);
    writer->SetInput(reader->GetOutput());
    writer->Update();

}

TEST(itkTensorFlow, tralala2) {

    char *argv[] = {
            (char*) "",
            (char*) "../../tests/testData/jpg/image2.jpg",
            (char*) "../../tests/testData/temp/samplePipeline2.png",
    };

    //using PixelType = itk::RGBPixel< unsigned char >;
    //using PixelType = unsigned char ;

    using PixelType = itk::Vector< unsigned char, 1 >;
    typedef itk::Image< PixelType, 2 > InputImageType;

    typedef itk::ImageFileReader<InputImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    PixelType *a= reader->GetOutput()->GetBufferPointer();
    unsigned char *b = (unsigned char*) a;
    std::cout << a[0] << std::endl;
    std::cout << (int)b[0] << " " << (int)b[1] << " " << (int)b[2] << " " << std::endl;
    //reader->GetOutput()->Print(std::cout);
    //reader->GetOutput(4);
    EXPECT_EQ(true, true);

}

TEST(itkTensorFlow, TF_DataTypeSize){

    //std::cout << typeid(float) << std::endl;
    std::cout << (typeid(std::float_t) == typeid(float)) << std::endl;
    std::cout << (typeid(std::int32_t) == typeid(float)) << std::endl;

    std::vector<std::string> stdTypeNames;
    stdTypeNames.push_back("");
    stdTypeNames.push_back(typeid(std::float_t).name());
    stdTypeNames.push_back(typeid(std::double_t).name());
    stdTypeNames.push_back(typeid(std::int32_t).name());
    stdTypeNames.push_back(typeid(std::uint8_t).name());
    stdTypeNames.push_back(typeid(std::int16_t).name());
    stdTypeNames.push_back(typeid(std::int8_t).name());
    stdTypeNames.push_back("");
    stdTypeNames.push_back(typeid(std::complex<float_t >).name());
    stdTypeNames.push_back(typeid(std::int64_t).name());
    stdTypeNames.push_back("");
    stdTypeNames.push_back("");
    stdTypeNames.push_back("");
    stdTypeNames.push_back("");
    stdTypeNames.push_back("");
    stdTypeNames.push_back("");
    stdTypeNames.push_back("");
    stdTypeNames.push_back(typeid(std::uint16_t).name());
    stdTypeNames.push_back(typeid(std::complex<double_t >).name());
    stdTypeNames.push_back("");
    stdTypeNames.push_back("");
    stdTypeNames.push_back("");
    stdTypeNames.push_back(typeid(std::uint32_t).name());
    stdTypeNames.push_back(typeid(std::uint64_t).name());

    size_t stdTypeSizes[]= {
            999,
            sizeof(std::float_t),
            sizeof(std::double_t),
            sizeof(std::int32_t),
            sizeof(std::uint8_t),
            sizeof(std::int16_t),
            sizeof(std::int8_t),
            999,
            sizeof(std::complex<float_t >),
            sizeof(std::int64_t),
            999,
            999,
            999,
            999,
            999,
            999,
            999,
            sizeof(std::uint16_t),
            sizeof(std::complex<double_t >),
            999,
            999,
            999,
            sizeof(std::uint32_t),
            sizeof(std::uint64_t)
    };

    std::cout << std::endl;
    for (int i = 1; i <= 23; i++) {
        std::cout << TFDataTypeToString((TF_DataType)i) << " \t\tsize: " << TF_DataTypeSize((TF_DataType)i)
                  << "\tstd: " << stdTypeNames[i] << " size: "<< stdTypeSizes[i] << std::endl;
    }

}





