//
// Created by Konrad Werys on 26/03/2019.
//

#ifndef ITKTENSORFLOW_TFPROCESSING_HXX
#define ITKTENSORFLOW_TFPROCESSING_HXX

#include "oxtfGraphReader.h"
#include "oxtfUtils.h"

namespace oxtf {

    class GraphRunner {

    public:

        GraphRunner();

        virtual ~GraphRunner() = default;

        int run();

        GraphReader *getGraphReader() const;
        void setGraphReader(GraphReader *_graphReader);

        TF_Tensor *getInputTensor() const;
        void setInputTensor(TF_Tensor *_inputTensor);

        TF_Tensor *getOutputTensor() const;
        void setOutputTensor(TF_Tensor *_outputTensor);

    private:

        GraphReader *_graphReader;
        TF_Tensor* _inputTensor;
        TF_Tensor* _outputTensor;

    };

} // namespace oxtf


#endif //ITKTENSORFLOW_TFPROCESSING_HXX
