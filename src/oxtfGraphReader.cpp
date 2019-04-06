//
// Created by Konrad Werys on 01/04/2019.
//

#include "oxtfGraphReader.h"

namespace oxtf {

    int GraphReader::readGraph() {

        // check path
        if (_graphPath.empty()) {
            std::cout << "Can't load graph" << std::endl;
            return 1; //EXIT_FAILURE
        }

        // try loading
        _graph = tf_utils::LoadGraph(_graphPath.c_str());

        // check if success
        if (_graph == nullptr) {
            std::cout << "Can't load graph" << std::endl;
            return 1; //EXIT_FAILURE
        }

        TF_Operation *op;
        std::size_t pos = 0;

        while ((op = TF_GraphNextOperation(_graph, &pos)) != nullptr) {
            const char *name = TF_OperationName(op);
            const char *type = TF_OperationOpType(op);
            const char *device = TF_OperationDevice(op);

            const int num_outputs = TF_OperationNumOutputs(op);
            const int num_inputs = TF_OperationNumInputs(op);

            // get input params
            if (_inputOperationDims == 0){
                if (num_outputs != 1){
                    std::cout << "GraphReader can only handle graphs with one input";
                    return 1; //EXIT_FAILURE
                }
                _inputOperationName = name;
                GetOpOutputInfo(_graph, op, 0, &_inputOperationDims, &_inputOperationSize, &_inputOperationType);
            }

            // get output params
            // TODO: find more pretty solution, currently overwriting the all output params with each operation.
            // Can i get the number of the operations in the graph?
            _outputOperationName = name;
            GetOpOutputInfo(_graph, op, 0, &_outputOperationDims, &_outputOperationSize, &_outputOperationType);

            // biggest x and y
            if (_outputOperationDims >= 3){
                if (       _outputOperationSize[1] > _operationWithBiggest2nd3rdSize[1]
                        && _outputOperationSize[2] > _operationWithBiggest2nd3rdSize[2] ){
                    _operationWithBiggest2nd3rdSize = _outputOperationSize;
                }
            }
        }

        return 0; //EXIT_SUCCESS
    }

    void
    GraphReader::setGraphPath(const std::string &_graphPath) {
        GraphReader::_graphPath = _graphPath;
    }

    const std::string &
    GraphReader::getGraphPath() const {
        return _graphPath;
    }

    TF_Graph *
    GraphReader::getGraph() const {
        return _graph;
    }

    const std::string &
    GraphReader::getInputOperationName() const {
        return _inputOperationName;
    }

    int64_t
    GraphReader::getInputOperationDims() const {
        return _inputOperationDims;
    }

    const std::vector<int64_t> &
    GraphReader::getInputOperationSize() const {
        return _inputOperationSize;
    }

    TF_DataType
    GraphReader::getInputOperationType() const {
        return _inputOperationType;
    }

    const std::string &
    GraphReader::getOutputOperationName() const {
        return _outputOperationName;
    }

    int64_t
    GraphReader::getOutputOperationDims() const {
        return _outputOperationDims;
    }

    const std::vector<int64_t> &
    GraphReader::getOutputOperationSize() const {
        return _outputOperationSize;
    }

    TF_DataType
    GraphReader::getOutputOperationType() const {
        return _outputOperationType;
    }

    const std::vector<int64_t> &
    GraphReader::getOperationWithBiggest2nd3rdSize() const {
        return _operationWithBiggest2nd3rdSize;
    }

    GraphReader::GraphReader() {
        _graphPath = "";

        _graph = nullptr;

        _inputOperationName = "";
        _inputOperationDims = 0;
        _inputOperationType = TF_FLOAT;

        _outputOperationName = "";
        _outputOperationDims = 0;
        _outputOperationType = TF_FLOAT;

        _operationWithBiggest2nd3rdSize.resize(3, -1);
    }

    GraphReader::~GraphReader() {
        tf_utils::DeleteGraph(_graph);
    }

    int
    GraphReader::GetOpOutputInfo(TF_Graph* graph, TF_Operation* op, int nth_output, std::int64_t *num_dims, std::vector<std::int64_t> *dims, TF_DataType *type) {

        TF_Status* status = TF_NewStatus();
        const int num_outputs = TF_OperationNumOutputs(op);

        if (num_outputs == 0){
            return 1; //EXIT_FAILURE
        }

        if (nth_output > num_outputs){
            std::cout << "Incorrect nth_output: " << nth_output << ". Number of outputs: " << num_outputs << std::endl;
            return 1; //EXIT_FAILURE
        }

        const TF_Output output = {op, nth_output};
        *type = TF_OperationOutputType(output);

        const std::int64_t temp_num_dims = TF_GraphGetTensorNumDims(graph, output, status);

        if (TF_GetCode(status) != TF_OK) {
            std::cout << "Can't get tensor dimensionality" << std::endl;
            return 1; //EXIT_FAILURE
        }

        *num_dims = temp_num_dims;

        std::vector<std::int64_t> temp_dims(*num_dims);
        TF_GraphGetTensorShape(graph, output, temp_dims.data(), *num_dims, status);

        if (TF_GetCode(status) != TF_OK) {
            std::cout << "Can't get get tensor shape" << std::endl;
            return 1; //EXIT_FAILURE
        }

        dims->clear();
        for (int j = 0; j < *num_dims; ++j) {
            dims->push_back(temp_dims[j]);
        }

        TF_DeleteStatus(status);

        return 0; //EXIT_SUCCESS
    }

} //namespace oxtf