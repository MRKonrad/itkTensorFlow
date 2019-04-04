//
// Created by Konrad Werys on 24/03/2019.
//

#include "gtest/gtest.h"

#include "oxtfUtils.h"


TEST(playground, printTensorFlowVersion_test) {

    EXPECT_NO_THROW(printTensorFlowVersion());

}

TEST(playground, printGraphInfo) {

    bool doPrint = false;

    if (!doPrint)
        std::cout.setstate(std::ios_base::failbit);

    EXPECT_NO_THROW(printGraphInfo("../../tests/testData/model_ocmr7.pb"));

    if (!doPrint)
        std::cout.setstate(std::ios_base::failbit);

}

TEST(playground, printGraphInfo2) {

    bool doPrint = false;

    if (!doPrint)
        std::cout.setstate(std::ios_base::failbit);

    EXPECT_NO_THROW(printGraphInfo("../../tests/testData/model_deeplab.pb"));

    if (!doPrint)
        std::cout.setstate(std::ios_base::failbit);

}

TEST(playground, sessionRun_model2) {

    long sizeX = 513, sizeY = 513, sizeZ = 1;
    const std::vector<std::int64_t> input_dims = {1, sizeX, sizeY, sizeZ};
    const std::vector<uint8_t> input_vals (sizeX * sizeY * sizeZ, 1);

    TF_Tensor* input_tensor = tf_utils::CreateTensor(TF_UINT8,
                                                     input_dims.data(), input_dims.size(),
                                                     input_vals.data(), input_vals.size() * sizeof(uint8_t));
    TF_Tensor* output_tensor = nullptr;

    EXPECT_NO_THROW(sessionRun(input_tensor, output_tensor, "../../tests/testData/model_deeplab.pb", "ImageTensor", "SemanticPredictions"));

    const auto data = static_cast<float*>(TF_TensorData(output_tensor));

    std::cout << "Output vals: " << data[0] << ", " << data[1] << ", " << data[2] << ", " << data[3] << std::endl;

    tf_utils::DeleteTensor(input_tensor);
    tf_utils::DeleteTensor(output_tensor);

}

TEST(playground, TF_DataTypeSize){

    bool doPrint = false;

    //std::cout << typeid(float) << std::endl;
    std::cout << (typeid(std::float_t) == typeid(float)) << std::endl;
    std::cout << (typeid(std::int32_t) == typeid(float)) << std::endl;

    std::vector<std::string> stdTypeNames;
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back(typeid(std::float_t).name());
    stdTypeNames.emplace_back(typeid(std::double_t).name());
    stdTypeNames.emplace_back(typeid(std::int32_t).name());
    stdTypeNames.emplace_back(typeid(std::uint8_t).name());
    stdTypeNames.emplace_back(typeid(std::int16_t).name());
    stdTypeNames.emplace_back(typeid(std::int8_t).name());
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back(typeid(std::complex<std::float_t >).name());
    stdTypeNames.emplace_back(typeid(std::int64_t).name());
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back(typeid(std::uint16_t).name());
    stdTypeNames.emplace_back(typeid(std::complex<std::double_t >).name());
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back("");
    stdTypeNames.emplace_back(typeid(std::uint32_t).name());
    stdTypeNames.emplace_back(typeid(std::uint64_t).name());

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

    if (!doPrint)
        std::cout.setstate(std::ios_base::failbit);

    std::cout << std::endl;
    for (int i = 1; i <= 23; i++) {
        std::cout << TFDataTypeToString((TF_DataType)i) << " \t\tsize: " << TF_DataTypeSize((TF_DataType)i)
                  << "\tstd: " << stdTypeNames[i] << " size: "<< stdTypeSizes[i] << std::endl;
    }

    if (!doPrint)
        std::cout.setstate(std::ios_base::failbit);

}





