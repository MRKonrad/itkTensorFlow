//
// Created by Konrad Werys on 08/05/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWAPI_H
#define ITKTENSORFLOW_ITKTENSORFLOWAPI_H

#include "itktensorflowlib_export.h"


ITKTENSORFLOWLIB_EXPORT int printTensorFlowVersion();

namespace oxtf {
    class ITKTENSORFLOWLIB_EXPORT PipelineBuilder;
}

namespace itk {
    template class ITKTENSORFLOWLIB_EXPORT Image<short, 3>;
    template class ITKTENSORFLOWLIB_EXPORT Image<int, 3>;
    template class ITKTENSORFLOWLIB_EXPORT Image<float, 3>;
    template class ITKTENSORFLOWLIB_EXPORT Image<double, 3>;

    template class ITKTENSORFLOWLIB_EXPORT TensorFlowImageFilter<typename Image<short, 3>, typename Image<short, 3> >;
    template class ITKTENSORFLOWLIB_EXPORT TensorFlowImageFilter<typename Image<int, 3>, typename Image<int, 3> >;
    template class ITKTENSORFLOWLIB_EXPORT TensorFlowImageFilter<typename Image<float, 3>, typename Image<float, 3> >;
    template class ITKTENSORFLOWLIB_EXPORT TensorFlowImageFilter<typename Image<double, 3>, typename Image<double, 3> >;
}

#endif //ITKTENSORFLOW_ITKTENSORFLOWAPI_H
