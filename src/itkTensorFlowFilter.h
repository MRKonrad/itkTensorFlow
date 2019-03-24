//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILER_H
#define ITKTENSORFLOW_ITKTENSORFLOWFILER_H

#include <iostream>
#include <c_api.h> // TensorFlow C API header

void printTensorFlowVersion(){
    std::cout << "TensorFlow Version: " << TF_Version() << std::endl;
}

#endif //ITKTENSORFLOW_ITKTENSORFLOWFILER_H
