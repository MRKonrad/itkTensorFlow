//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILER_H
#define ITKTENSORFLOW_ITKTENSORFLOWFILER_H

#include <iostream>
#include <c_api.h> // TensorFlow C API header
#include "graphInfo.h"

int printTensorFlowVersion(){
    std::cout << "TensorFlow Version: " << TF_Version() << std::endl;
    return 0; //EXIT_SUCCESS
}

int printGraphInfo(std::string graphPath){
    TF_Graph* graph = tf_utils::LoadGraph(graphPath.c_str());
    if (graph == nullptr) {
        std::cout << "Can't load graph" << std::endl;
        return 1; //EXIT_FAILURE
    }

    PrintOp(graph);

    tf_utils::DeleteGraph(graph);
    return 0; //EXIT_SUCCESS
}

template <class T>
int sessionRun(TF_DataType myTF_DataType, long sizeX, long sizeY, long sizeZ, std::string graphPath, std::string inputNodeName, std::string outputNodeName){
    TF_Graph* graph = tf_utils::LoadGraph(graphPath.c_str());
    if (graph == nullptr) {
        std::cout << "Can't load graph" << std::endl;
        return 1;
    }

    TF_Output input_op = {TF_GraphOperationByName(graph, inputNodeName.c_str()), 0};
    if (input_op.oper == nullptr) {
        std::cout << "Can't init input_op" << std::endl;
        return 2;
    }

    const std::vector<std::int64_t> input_dims = {1, sizeX, sizeY, sizeZ};
//    const std::vector<float> input_vals (sizeX * sizeY * sizeZ, 1.f);
//
//    TF_Tensor* input_tensor = tf_utils::CreateTensor(TF_FLOAT,
//                                                     input_dims.data(), input_dims.size(),
//                                                     input_vals.data(), input_vals.size() * sizeof(float));

    const std::vector<T> input_vals (sizeX * sizeY * sizeZ, 1);

    TF_Tensor* input_tensor = tf_utils::CreateTensor(myTF_DataType,
                                                     input_dims.data(), input_dims.size(),
                                                     input_vals.data(), input_vals.size() * sizeof(T));



    TF_Output out_op = {TF_GraphOperationByName(graph, outputNodeName.c_str()), 0};
    if (out_op.oper == nullptr) {
        std::cout << "Can't init out_op" << std::endl;
        return 3;
    }

    TF_Tensor* output_tensor = nullptr;

    TF_Status* status = TF_NewStatus();
    TF_SessionOptions* options = TF_NewSessionOptions();
    TF_Session* sess = TF_NewSession(graph, options, status);
    TF_DeleteSessionOptions(options);

    if (TF_GetCode(status) != TF_OK) {
        TF_DeleteStatus(status);
        return 4;
    }

    TF_SessionRun(sess,
                  nullptr, // Run options.
                  &input_op, &input_tensor, 1, // Input tensors, input tensor values, number of inputs.
                  &out_op, &output_tensor, 1, // Output tensors, output tensor values, number of outputs.
                  nullptr, 0, // Target operations, number of targets.
                  nullptr, // Run metadata.
                  status // Output status.
    );

    if (TF_GetCode(status) != TF_OK) {
        std::cout << "Error run session";
        TF_DeleteStatus(status);
        return 5;
    }

    TF_CloseSession(sess, status);
    if (TF_GetCode(status) != TF_OK) {
        std::cout << "Error close session";
        TF_DeleteStatus(status);
        return 6;
    }

    TF_DeleteSession(sess, status);
    if (TF_GetCode(status) != TF_OK) {
        std::cout << "Error delete session";
        TF_DeleteStatus(status);
        return 7;
    }

    const auto data = static_cast<float*>(TF_TensorData(output_tensor));

    std::cout << "Output vals: " << data[0] << ", " << data[1] << ", " << data[2] << ", " << data[3] << std::endl;

    tf_utils::DeleteGraph(graph);
    tf_utils::DeleteTensor(input_tensor);
    tf_utils::DeleteTensor(output_tensor);
    TF_DeleteStatus(status);

    return 0;
}

#endif //ITKTENSORFLOW_ITKTENSORFLOWFILER_H
