//
// Created by Konrad Werys on 06/04/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWAPI_H
#define ITKTENSORFLOW_ITKTENSORFLOWAPI_H

#include "itktensorflowlib_export.h"
#include <string>
#include "oxtfPipelineBuilder.h"

ITKTENSORFLOWLIB_EXPORT int printTensorFlowVersion();

namespace oxtf {
    class ITKTENSORFLOWLIB_EXPORT PipelineBuilder;
}

#endif //ITKTENSORFLOW_ITKTENSORFLOWAPI_H
