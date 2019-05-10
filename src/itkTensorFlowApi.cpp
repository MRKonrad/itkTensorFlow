//
// Created by Konrad Werys on 08/05/2019.
//

#include "itkTensorFlowApi.h"
#include "itkTensorFlowFilter.h"
#include "oxtfPipelineBuilder.h"

#ifndef ITKTENSORFLOWLIB_EXPORT
#define ITKTENSORFLOWLIB_EXPORT
#endif

//ITKTENSORFLOWLIB_EXPORT int printTensorFlowVersion();

class ITKTENSORFLOWLIB_EXPORT oxtf::PipelineBuilder;

template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<short, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<int, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<float, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<double, 3> >;

#undef ITKTENSORFLOWLIB_EXPORT