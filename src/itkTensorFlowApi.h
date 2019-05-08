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

template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < itk::Image<short, 3>, itk::Image<short, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < itk::Image<int, 3>, itk::Image<int, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < itk::Image<float, 3>, itk::Image<float, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < itk::Image<double, 3>, itk::Image<double, 3> >;

#endif //ITKTENSORFLOW_ITKTENSORFLOWAPI_H
