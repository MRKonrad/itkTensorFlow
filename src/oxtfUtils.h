//
// Created by Konrad Werys on 25/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTFUTILS_H
#define ITKTENSORFLOW_ITKTFUTILS_H


#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <complex>
#include <c_api.h> // TensorFlow C API header
#include "itktensorflowlib_export.h"
#include "interp.h"

/**
 * based on https://github.com/Neargye/hello_tf_c_api
 * @param data_type
 * @return
 */
ITKTENSORFLOWLIB_EXPORT const char* TFDataTypeToString(TF_DataType data_type);

/**
 * based on https://github.com/Neargye/hello_tf_c_api
 * @param op
 */
ITKTENSORFLOWLIB_EXPORT void PrintOpInputs(TF_Graph*, TF_Operation* op);

/**
 * based on https://github.com/Neargye/hello_tf_c_api
 * @param graph
 * @param op
 */
ITKTENSORFLOWLIB_EXPORT void PrintOpOutputs(TF_Graph* graph, TF_Operation* op);

/**
 * based on https://github.com/Neargye/hello_tf_c_api
 * @param graph
 */
ITKTENSORFLOWLIB_EXPORT void PrintOp(TF_Graph* graph);

/**
 * based on https://github.com/Neargye/hello_tf_c_api
 * @return
 */
ITKTENSORFLOWLIB_EXPORT int printTensorFlowVersion();

/**
 * based on https://github.com/Neargye/hello_tf_c_api
 * @param graphPath
 * @return
 */
ITKTENSORFLOWLIB_EXPORT int printGraphInfo(std::string graphPath);

/**
 * based on https://github.com/Neargye/hello_tf_c_api
 * @param input_tensor
 * @param output_tensor
 * @param graphPath
 * @param inputNodeName
 * @param outputNodeName
 * @return
 */
ITKTENSORFLOWLIB_EXPORT int sessionRun(TF_Tensor* input_tensor, TF_Tensor*& output_tensor, std::string graphPath, std::string inputNodeName, std::string outputNodeName);

inline char fileSeparator() {
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

#endif //ITKTENSORFLOW_ITKTFUTILS_H
