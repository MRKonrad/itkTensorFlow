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
            int temp = sessionRun(
                    _inputTensor,
                    _outputTensor,
                    _graphReader->getGraphPath(),
                    _graphReader->getInputOperationName(),
                    _graphReader->getOutputOperationName());

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
