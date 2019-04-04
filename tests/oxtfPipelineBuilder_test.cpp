//
// Created by Konrad Werys on 04/04/2019.
//

#include "gtest/gtest.h"
#include "oxtfPipelineBuilder.h"

TEST(PipelineBuilder, PipelineBuilder_noImagePaths) {

    oxtf::PipelineBuilder pipelineBuilder;

    EXPECT_THROW(pipelineBuilder.runPipeline(), std::runtime_error);

}


