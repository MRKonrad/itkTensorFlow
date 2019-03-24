//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILER_H
#define ITKTENSORFLOW_ITKTENSORFLOWFILER_H

#include <iostream>
#include <c_api.h> // TensorFlow C API header
#include "graphInfo.h"

void printTensorFlowVersion(){
    std::cout << "TensorFlow Version: " << TF_Version() << std::endl;
}

void printGraphInfo(std::string graphPath){
    TF_Graph* graph = tf_utils::LoadGraph(graphPath.c_str());
    if (graph == nullptr) {
        std::cout << "Can't load graph" << std::endl;
        return;
    }

    PrintOp(graph);

    tf_utils::DeleteGraph(graph);
}

#endif //ITKTENSORFLOW_ITKTENSORFLOWFILER_H
