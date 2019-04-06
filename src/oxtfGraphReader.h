//
// Created by Konrad Werys on 31/03/2019.
//

#ifndef ITKTENSORFLOW_TFMODELREADER_H
#define ITKTENSORFLOW_TFMODELREADER_H


#include <iostream>
#include <vector>
#include <string>
#include <c_api.h> // TensorFlow C API header

namespace oxtf {

    class GraphReader {

    public:

        // the most important function
        int readGraph();

        void setGraphPath(const std::string &_graphPath);
        const std::string &getGraphPath() const;

        TF_Graph *getGraph() const;

        const std::string &getInputOperationName() const;
        int64_t getInputOperationDims() const;

        const std::vector<int64_t> &getInputOperationSize() const;
        TF_DataType getInputOperationType() const;

        const std::string &getOutputOperationName() const;
        int64_t getOutputOperationDims() const;

        const std::vector<int64_t> &getOutputOperationSize() const;
        TF_DataType getOutputOperationType() const;

        const std::vector<int64_t> &getOperationWithBiggest2nd3rdSize() const;

        int64_t getMaxX() const{
            return _operationWithBiggest2nd3rdSize[1];
        }

        int64_t getMaxY() const{
            return _operationWithBiggest2nd3rdSize[2];
        }

        // constuctor
        GraphReader();

        //destructor
        virtual ~GraphReader();

    private:

        std::string _graphPath;

        TF_Graph *_graph;

        std::string _inputOperationName;
        int64_t _inputOperationDims;
        std::vector<std::int64_t> _inputOperationSize;
        TF_DataType _inputOperationType;

        std::string _outputOperationName;
        int64_t _outputOperationDims;
        std::vector<std::int64_t> _outputOperationSize;
        TF_DataType _outputOperationType;

        std::vector<std::int64_t> _operationWithBiggest2nd3rdSize;

        int GetOpOutputInfo(
                TF_Graph* graph,
                TF_Operation* op,
                int nth_output,
                std::int64_t *num_dims,
                std::vector<std::int64_t> *dims,
                TF_DataType *type);
    };

} // namespace oxtf

#endif //ITKTENSORFLOW_TFMODELREADER_H
