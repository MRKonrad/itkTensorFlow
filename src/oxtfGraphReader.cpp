//
// Created by Konrad Werys on 01/04/2019.
//

#include "oxtfGraphReader.h"

#include "oxtfUtils.h"


namespace oxtf {

    int GraphReader::readGraph() {

        // check path
        if (_graphPath.empty()) {
            std::cerr << "Can't load graph" << std::endl;
            return 1; //EXIT_FAILURE
        }

        // try loading
        _graph = tf_utils::LoadGraph(_graphPath.c_str());

        // check if success
        if (_graph == nullptr) {
            std::cerr << "Can't load graph" << std::endl;
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

            int64_t tempOpDims;
            std::vector<std::int64_t> tempOpSize;
            TF_DataType tempOpType;


            int result = GetOpOutputInfo(_graph, op, 0, &tempOpDims, &tempOpSize, &tempOpType);
            if (result != 0)
                continue;

            // ----------------------------
            // --- getting input params ---
            // ----------------------------

            if (    ( _inputOperationDims == 0 ) // skip if already set
                 && ( tempOpDims == 4 ) ) { // consider only ops with 4 dims

                if (   (tempOpSize[1] > 1 || tempOpSize[1] == -1) // consider only x == -1 or x > 1
                    && (tempOpSize[2] > 1 || tempOpSize[2] == -1)) { // consider only y == -1 or y > 1

                    if (num_outputs == 1) {
                        _inputOperationName = name;
                        _inputOperationDims = tempOpDims;
                        _inputOperationSize = tempOpSize;
                        _inputOperationType = tempOpType;

                    }
                }
            }

            // -----------------------------
            // --- getting output params ---
            // -----------------------------

            // TODO: find more pretty solution, currently overwriting the all output params with each operation.
            // Can i get the number of the operations in the graph?
            _outputOperationName = name;
            _outputOperationDims = tempOpDims;
            _outputOperationSize = tempOpSize;
            _outputOperationType = tempOpType;

            // biggest x and y
            if (tempOpDims >= 3){
                if (       tempOpSize[1] > _operationWithBiggest2nd3rdSize[1]
                        && tempOpSize[2] > _operationWithBiggest2nd3rdSize[2] ){
                    _operationWithBiggest2nd3rdSize = tempOpSize;
                }
            }
        }

        // if has not been set print to cerr
        if (_inputOperationDims == 0){
            std::cerr << "inputOperation has not been found" << std::endl;
            return 1; // EXIT_FAILURE
        }

        // if has not been set print to cerr
        if (_outputOperationDims == 0){
            std::cerr << "outputOperation has not been found" << std::endl;
            return 1; // EXIT_FAILURE
        }

        std::cout << "inputOperationName: " << _inputOperationName << std::endl;
        std::cout << "outputOperationName: " << _outputOperationName << std::endl;

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

    void GraphReader::disp(){
        std::cout << "GraphReader" << std::endl;
        std::cout << "  graphPath: " << _graphPath << std::endl;
        std::cout << "  inputOperationName: " << _inputOperationName
                  << " inputOperationType: " << TFDataTypeToString(_inputOperationType);
        std::cout << " inputOperationSize: [ ";
        for (int i = 0; i < _inputOperationSize.size(); ++i){
            std::cout << _inputOperationSize[i] << " ";
        }
        std::cout << " ]" << std::endl;

        std::cout << "  outputOperationName: " << _outputOperationName
                  << " outputOperationDims: " << _outputOperationDims
                  << " outputOperationType: " << TFDataTypeToString(_outputOperationType);
        std::cout << " outputOperationSize: [ ";
        for (int i = 0; i < _outputOperationSize.size(); ++i){
            std::cout << _outputOperationSize[i] << " ";
        }
        std::cout << " ]" << std::endl;

        std::cout << "  operationWithBiggest2nd3rdSize: [ ";
        for (int i = 0; i < _operationWithBiggest2nd3rdSize.size(); ++i){
            std::cout << _operationWithBiggest2nd3rdSize[i] << " ";
        }
        std::cout << " ]"<< std::endl;
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

        if (temp_num_dims < 0){
            //std::cout << "Tensor dim < 0" << std::endl;
            return 2; //EXIT_FAILURE
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