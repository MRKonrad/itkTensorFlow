//
// Created by Konrad Werys on 24/03/2019.
//

#include "oxtfPipelineBuilder.h"
#include "oxtfUtils.h"
#include "getopt.h"
#include <iostream>

void printInfo(){
    printf("\nUse: itkTensorFlowExe\n"
           "\t -r inputRgbFilePath\n"
           "\t -g inputGrayFilePaths, can be used multiple times\n"
           "\t -m inputGraphPath tensor flow model/graph (in .pb format)\n"
           "\t -o outputDirPath\n"
           "\t -s scale (multiply output values by this value)\n"
           "\t -h help\n"
           "Enjoy!\n"
           "author: Konrad Werys 2019 konrad.werys@cardiov.ox.ac.uk\n\n");
}

int main(int argc, char** argv) {

    printTensorFlowVersion();

    std::string inputRgbFilePath;
    std::string inputGraphPath;
    std::string outputDirPath;
    std::vector<std::string> inputGrayFilePaths;
    float scale = 1;

    // **************
    // *** PARSER ***
    // **************

    if (argc == 2){
        printInfo();
        return 1; // EXIT_FAILURE
    }

    opterr = 0;

    // Retrieve the options:
    int c;
    while ((c = getopt (argc, argv, "r:g:m:o:s:h")) != -1){
        switch (c) {
            case 'r':
                inputRgbFilePath = optarg;
                break;
            case 'g':
                inputGrayFilePaths.emplace_back(std::string(optarg));
                break;
            case 'm':
                inputGraphPath = optarg;
                break;
            case 'o':
                outputDirPath = optarg;
                break;
            case 's':
                scale = std::stoi(optarg);
                break;
            case 'h':
                printInfo();
                return 1; // EXIT_FAILURE
            case '?':
                if (optopt == 'i' || optopt == 'g' || optopt == 'm' || optopt == 'o')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }
    }

    // ***********************
    // *** PipelineBuilder ***
    // ***********************

    oxtf::PipelineBuilder pipelineBuilder;
    pipelineBuilder.setInputImageRgbPath(inputRgbFilePath);
    pipelineBuilder.setInputImagesGrayscalePaths(inputGrayFilePaths);
    pipelineBuilder.setOutputDirPath(outputDirPath);
    pipelineBuilder.setGraphPath(inputGraphPath);
    pipelineBuilder.setMultiplyOutputByFactor(scale);
    pipelineBuilder.disp();

    pipelineBuilder.runPipeline();

    return 0;
}
