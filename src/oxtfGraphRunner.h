//
// Created by Konrad Werys on 26/03/2019.
//

#ifndef ITKTENSORFLOW_TFPROCESSING_HXX
#define ITKTENSORFLOW_TFPROCESSING_HXX

#include "oxtfGraphReader.h"
#include "itktfUtils.h"

// TODO: move implementation to .cpp file

namespace oxtf {

    class GraphRunner {

    public:

        GraphRunner(){
            _graphReader = nullptr;
            _inputTensor = nullptr;
            _outputTensor = nullptr;
        }

        virtual ~GraphRunner() = default;

        int run(){

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

        GraphReader *getGraphReader() const {
            return _graphReader;
        }

        void setGraphReader(GraphReader *_graphReader) {
            GraphRunner::_graphReader = _graphReader;
        }

        TF_Tensor *getInputTensor() const {
            return _inputTensor;
        }

        void setInputTensor(TF_Tensor *_inputTensor) {
            GraphRunner::_inputTensor = _inputTensor;
        }

        TF_Tensor *getOutputTensor() const {
            return _outputTensor;
        }

        void setOutputTensor(TF_Tensor *_outputTensor) {
            GraphRunner::_outputTensor = _outputTensor;
        }

    private:

        GraphReader *_graphReader;
        TF_Tensor* _inputTensor;
        TF_Tensor* _outputTensor;

    };

} // namespace oxtf


#endif //ITKTENSORFLOW_TFPROCESSING_HXX
