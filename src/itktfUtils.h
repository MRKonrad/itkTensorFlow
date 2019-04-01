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

const char* TFDataTypeToString(TF_DataType data_type);

void PrintOpInputs(TF_Graph*, TF_Operation* op);

void PrintOpOutputs(TF_Graph* graph, TF_Operation* op);

void PrintOp(TF_Graph* graph);

int printTensorFlowVersion();

int printGraphInfo(std::string graphPath);

int sessionRun(TF_Tensor* input_tensor, TF_Tensor*& output_tensor, std::string graphPath, std::string inputNodeName, std::string outputNodeName);

inline char fileSeparator()
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

#endif //ITKTENSORFLOW_ITKTFUTILS_H
