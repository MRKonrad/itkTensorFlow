//
// Created by Konrad Werys on 26/03/2019.
//

#include "oxtfGraphRunner.h"
#include <chrono>
#include "oxtfUtils.h"

namespace oxtf {

    int
    GraphRunner::run(){

        // check types
        if (TF_TensorType(_inputTensor) != _graphReader->getInputOperationType()){
            std::cerr << "GraphRunner:run this graph needs: " << TFDataTypeToString(_graphReader->getInputOperationType()) << " as input type" << std::endl;
            return 1; // EXIT_FAILURE
        }

        // check sizes
        for (int iDim = 0; iDim < TF_NumDims(_inputTensor); iDim++){
            if ( _graphReader->getInputOperationSize()[iDim] == -1){
                continue;
            }
            if (_graphReader->getInputOperationSize()[iDim] != TF_Dim(_inputTensor, iDim)){
                std::cerr << "GraphRunner:run input tensor dim[" << iDim << "] = " << TF_Dim(_inputTensor, iDim) <<
                          " is different than graph dim[" << iDim << "] = " << _graphReader->getInputOperationSize()[iDim] << std::endl;
                return 1; // EXIT_FAILURE
            }
        }

        //auto start = std::chrono::high_resolution_clock::now();

        int temp = sessionRun(
                _inputTensor,
                _outputTensor,
                _graphReader->getGraphPath(),
                _graphReader->getInputOperationName(),
                _graphReader->getOutputOperationName());

        //auto stop = std::chrono::high_resolution_clock::now();
        //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        //std::cout << "GraphRunner::run time: " << float(duration.count())/1000. << " seconds. " << std::endl;

        if (TF_TensorType(_outputTensor) != _graphReader->getOutputOperationType()){
            std::cerr << "GraphRunner:run: this graph needs: " << TFDataTypeToString(_graphReader->getOutputOperationType()) << " as output type" << std::endl;
            return 1; // EXIT_FAILURE
        }

        return temp;
    }

    const GraphReader *
    GraphRunner::getGraphReader() const {
        return _graphReader;
    }

    void
    GraphRunner::setGraphReader(const GraphReader *_graphReader) {
        GraphRunner::_graphReader = _graphReader;
    }

    TF_Tensor *
    GraphRunner::getInputTensor() const {
        return _inputTensor;
    }

    void
    GraphRunner::setInputTensor(TF_Tensor *_inputTensor) {
        GraphRunner::_inputTensor = _inputTensor;
    }

    TF_Tensor *
    GraphRunner::getOutputTensor() const {
        return _outputTensor;
    }

    void
    GraphRunner::setOutputTensor(TF_Tensor *_outputTensor) {
        GraphRunner::_outputTensor = _outputTensor;
    }


    GraphRunner::GraphRunner(){
        _graphReader = nullptr;
        _inputTensor = nullptr;
        _outputTensor = nullptr;
    }

} // namespace oxtf