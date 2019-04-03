//
// Created by Konrad Werys on 26/03/2019.
//

#include "oxtfGraphRunner.h"

namespace oxtf {

    int
    GraphRunner::run(){

        if (TF_TensorType(_inputTensor) != _graphReader->getInputOperationType()){
            std::cout << "GraphRunner:run: this graph needs: " << TFDataTypeToString(_graphReader->getInputOperationType()) << " as input type" << std::endl;
            return 1; // EXIT_FAILURE
        }

        int temp = sessionRun(
                _inputTensor,
                _outputTensor,
                _graphReader->getGraphPath(),
                _graphReader->getInputOperationName(),
                _graphReader->getOutputOperationName());

        if (TF_TensorType(_outputTensor) != _graphReader->getOutputOperationType()){
            std::cout << "GraphRunner:run: this graph needs: " << TFDataTypeToString(_graphReader->getOutputOperationType()) << " as output type" << std::endl;
            return 1; // EXIT_FAILURE
        }

        return temp;
    }

    GraphReader *
    GraphRunner::getGraphReader() const {
        return _graphReader;
    }

    void
    GraphRunner::setGraphReader(GraphReader *_graphReader) {
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