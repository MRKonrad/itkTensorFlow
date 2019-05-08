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
    template< typename TImageIn, int dim>
    class Image <TImageIn, dim>;

    typedef Image< short, 3 > ImageTypeShort;
    typedef Image< int, 3 > ImageTypeInt;
    typedef Image< float, 3 > ImageTypeFloat;
    typedef Image< double, 3 > ImageTypeDouble;

    template class ITKTENSORFLOWLIB_EXPORT TensorFlowImageFilter< ImageTypeShort, ImageTypeShort >;
    template class ITKTENSORFLOWLIB_EXPORT TensorFlowImageFilter< ImageTypeInt, ImageTypeInt >;
    template class ITKTENSORFLOWLIB_EXPORT TensorFlowImageFilter< ImageTypeFloat, ImageTypeFloat >;
    template class ITKTENSORFLOWLIB_EXPORT TensorFlowImageFilter< ImageTypeDouble, ImageTypeDouble >;
}

#endif //ITKTENSORFLOW_ITKTENSORFLOWAPI_H
