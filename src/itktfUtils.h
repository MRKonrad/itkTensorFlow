//
// Created by Konrad Werys on 25/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTFUTILS_H
#define ITKTENSORFLOW_ITKTFUTILS_H

#include "tf_utils.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <c_api.h> // TensorFlow C API header

const char* TFDataTypeToString(TF_DataType data_type) {
    switch (data_type) {
        case TF_FLOAT:
            return "TF_FLOAT";
        case TF_DOUBLE:
            return "TF_DOUBLE";
        case TF_INT32:
            return "TF_INT32";
        case TF_UINT8:
            return "TF_UINT8";
        case TF_INT16:
            return "TF_INT16";
        case TF_INT8:
            return "TF_INT8";
        case TF_STRING:
            return "TF_STRING";
        case TF_COMPLEX64:
            return "TF_COMPLEX64";
        case TF_INT64:
            return "TF_INT64";
        case TF_BOOL:
            return "TF_BOOL";
        case TF_QINT8:
            return "TF_QINT8";
        case TF_QUINT8:
            return "TF_QUINT8";
        case TF_QINT32:
            return "TF_QINT32";
        case TF_BFLOAT16:
            return "TF_BFLOAT16";
        case TF_QINT16:
            return "TF_QINT16";
        case TF_QUINT16:
            return "TF_QUINT16";
        case TF_UINT16:
            return "TF_UINT16";
        case TF_COMPLEX128:
            return "TF_COMPLEX128";
        case TF_HALF:
            return "TF_HALF";
        case TF_RESOURCE:
            return "TF_RESOURCE";
        case TF_VARIANT:
            return "TF_VARIANT";
        case TF_UINT32:
            return "TF_UINT32";
        case TF_UINT64:
            return "TF_UINT64";
        default:
            return "Unknown";
    }
}

void PrintOpInputs(TF_Graph*, TF_Operation* op) {
    const int num_inputs = TF_OperationNumInputs(op);

    std::cout << "Number inputs: " << num_inputs << std::endl;

    for (int i = 0; i < num_inputs; ++i) {
        const TF_Input input = {op, i};
        const TF_DataType type = TF_OperationInputType(input);
        std::cout << std::to_string(i) << " type : " << TFDataTypeToString(type) << std::endl;
    }
}

void PrintOpOutputs(TF_Graph* graph, TF_Operation* op) {
    TF_Status* status = TF_NewStatus();
    const int num_outputs = TF_OperationNumOutputs(op);

    std::cout << "Number outputs: " << num_outputs << std::endl;

    for (int i = 0; i < num_outputs; ++i) {
        const TF_Output output = {op, i};
        const TF_DataType type = TF_OperationOutputType(output);
        std::cout << std::to_string(i) << " type : " << TFDataTypeToString(type);

        const int num_dims = TF_GraphGetTensorNumDims(graph, output, status);

        if (TF_GetCode(status) != TF_OK) {
            std::cout << "Can't get tensor dimensionality" << std::endl;
            continue;
        }

        std::cout << " dims: " << num_dims;

        if (num_dims <= 0) {
            std::cout << " []" << std::endl;;
            continue;
        }

        std::vector<std::int64_t> dims(num_dims);
        TF_GraphGetTensorShape(graph, output, dims.data(), num_dims, status);

        if (TF_GetCode(status) != TF_OK) {
            std::cout << "Can't get get tensor shape" << std::endl;
            continue;
        }

        std::cout << " [";
        for (int j = 0; j < num_dims; ++j) {
            std::cout << dims[j];
            if (j < num_dims - 1) {
                std::cout << ",";
            }
        }
        std::cout << "]" << std::endl;
    }

    TF_DeleteStatus(status);
}

void PrintOp(TF_Graph* graph) {
    TF_Operation* op;
    std::size_t pos = 0;

    while ((op = TF_GraphNextOperation(graph, &pos)) != nullptr) {
        const char* name = TF_OperationName(op);
        const char* type = TF_OperationOpType(op);
        const char* device = TF_OperationDevice(op);

        const int num_outputs = TF_OperationNumOutputs(op);
        const int num_inputs = TF_OperationNumInputs(op);

        std::cout << pos << ": " << name << " type: " << type << " device: " << device << " number inputs: " << num_inputs << " number outputs: " << num_outputs << std::endl;

        PrintOpInputs(graph, op);
        PrintOpOutputs(graph, op);
        std::cout << std::endl;
    }
}

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

int sessionRun(TF_Tensor* input_tensor, TF_Tensor*& output_tensor, std::string graphPath, std::string inputNodeName, std::string outputNodeName){

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


    TF_Output out_op = {TF_GraphOperationByName(graph, outputNodeName.c_str()), 0};
    if (out_op.oper == nullptr) {
        std::cout << "Can't init out_op" << std::endl;
        return 3;
    }

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

    tf_utils::DeleteGraph(graph);

    TF_DeleteStatus(status);

    return 0;
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

inline char fileSeparator()
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

#endif //ITKTENSORFLOW_ITKTFUTILS_H
