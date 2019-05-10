//
// Created by Konrad Werys on 08/05/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWAPI_H
#define ITKTENSORFLOW_ITKTENSORFLOWAPI_H

#include "itktensorflowlib_export.h"
#include "itkTensorFlowFilter.h"
#include "oxtfImageToTensor.h"
#include "oxtfTensorToItkImage.h"

//namespace oxtf {
//    class ITKTENSORFLOWLIB_EXPORT GraphReader;
//    class ITKTENSORFLOWLIB_EXPORT GraphRunner;
//    class ITKTENSORFLOWLIB_EXPORT PipelineBuilder;
//}

template class ITKTENSORFLOWLIB_EXPORT oxtf::TensorToImage < typename itk::Image<short, 3> >;
template class ITKTENSORFLOWLIB_EXPORT oxtf::TensorToImage < typename itk::Image<int, 3> >;
template class ITKTENSORFLOWLIB_EXPORT oxtf::TensorToImage < typename itk::Image<float, 3> >;
template class ITKTENSORFLOWLIB_EXPORT oxtf::TensorToImage < typename itk::Image<double, 3> >;

template class ITKTENSORFLOWLIB_EXPORT oxtf::ImageToTensor < typename itk::Image<short, 3> >;
template class ITKTENSORFLOWLIB_EXPORT oxtf::ImageToTensor < typename itk::Image<int, 3> >;
template class ITKTENSORFLOWLIB_EXPORT oxtf::ImageToTensor < typename itk::Image<float, 3> >;
template class ITKTENSORFLOWLIB_EXPORT oxtf::ImageToTensor < typename itk::Image<double, 3> >;

template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<short, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<int, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<float, 3> >;
template class ITKTENSORFLOWLIB_EXPORT itk::TensorFlowImageFilter < typename itk::Image<double, 3> >;

#endif //ITKTENSORFLOW_ITKTENSORFLOWAPI_H
