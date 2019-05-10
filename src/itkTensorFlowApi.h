//
// Created by Konrad Werys on 08/05/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWAPI_H
#define ITKTENSORFLOW_ITKTENSORFLOWAPI_H

#include "itktensorflowlib_export.h"
#include "itkTensorFlowFilter.h"
//#include "oxtfPipelineBuilder.h"
//#include "oxtfUtils.h"

//ITKTENSORFLOWLIB_EXPORT int printTensorFlowVersion();
//
//ITKTENSORFLOWLIB_EXPORT class oxtf::PipelineBuilder;

#ifndef ITKTENSORFLOWLIB_EXPORT
#define ITKTENSORFLOWLIB_EXPORT
#endif

template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<short, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<int, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<float, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<double, 3> >;

#undef ITKTENSORFLOWLIB_EXPORT

#endif //ITKTENSORFLOW_ITKTENSORFLOWAPI_H
