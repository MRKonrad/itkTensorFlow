//
// Created by Konrad Werys on 26/03/2019.
//

#ifndef ITKTENSORFLOW_TFPROCESSING_HXX
#define ITKTENSORFLOW_TFPROCESSING_HXX

#include "oxtfGraphReader.h"
#include "itktensorflowlib_export.h"

namespace oxtf {

    class ITKTENSORFLOWLIB_EXPORT GraphRunner {

    public:

        GraphRunner();

        virtual ~GraphRunner() = default;

        int run();

        const GraphReader *getGraphReader() const;
        void setGraphReader(const GraphReader *_graphReader);

        TF_Tensor *getInputTensor() const;
        void setInputTensor(TF_Tensor *_inputTensor);

        TF_Tensor *getOutputTensor() const;
        void setOutputTensor(TF_Tensor *_outputTensor);

    private:

        const GraphReader *_graphReader;
        TF_Tensor* _inputTensor;
        TF_Tensor* _outputTensor;

    };

} // namespace oxtf


#endif //ITKTENSORFLOW_TFPROCESSING_HXX
