//
// Created by Konrad Werys on 04/04/2019.
//

#include "oxtfPipelineBuilder.h"

namespace oxtf {

    int
    PipelineBuilder
    ::runPipeline (){

        typedef float PixelTypeIn;
        typedef itk::Image<PixelTypeIn, 3> ImageType;
        ImageType* image;
        oxtf::GraphReader *graphReader;

        graphReader = graphReaderMaker(_graph_path);

        image = readInputImage<ImageType>();

        if (_doPadding) {
            image = padImage(image, graphReader->getMaxX(), graphReader->getMaxY());
        }

        if (_flipAxes[0] || _flipAxes[1] || _flipAxes[2]){
            image = flipImage(image, _flipAxes);
        }

        if (_threshold != NAN){
            image = thresholdImage(image, _threshold);
        }

        image = runGraphOnImage(image, graphReader);

        writeImages(image, _output_dir_path);

        delete graphReader;

        return 0; // EXIT_SUCCESS
    }

    PipelineBuilder
    ::PipelineBuilder(){
        _doPadding = true;
        _flipAxes = std::vector<bool>(3, false);
        _threshold = NAN;
    };

    oxtf::GraphReader *
    PipelineBuilder
    ::graphReaderMaker(const std::string &graph_path) {

        auto *graphReader = new oxtf::GraphReader();
        graphReader->setGraphPath(graph_path);

        if (graphReader->readGraph() != 0) { //needs exit success
            delete graphReader;
            return nullptr;
        }

        return graphReader;

    }

    template< typename TImage>
    TImage*
    PipelineBuilder
    ::readInputImage(){

        if (_input_images_grayscale_paths.empty() && _input_image_rgb_path.empty()){
            throw std::runtime_error("Provide input_images_grayscale_paths OR input_image_rgb_path");
        }
        if (!_input_images_grayscale_paths.empty() && !_input_image_rgb_path.empty()){
            throw std::runtime_error("Provide input_images_grayscale_paths OR input_image_rgb_path, not both");
        }

        if        (!_input_images_grayscale_paths.empty()) {
            return readInputImageRgb<TImage>(_input_image_rgb_path);
        } else if (!_input_image_rgb_path.empty()){
            return readInputImageGray<TImage>(_input_images_grayscale_paths);
        }

        return nullptr;
    }

    template< typename TImage>
    TImage*
    PipelineBuilder
    ::readInputImageRgb(const std::string &image_path){
        return nullptr;
    }

    template< typename TImage>
    TImage*
    PipelineBuilder
    ::readInputImageGray(const std::vector<std::string> &images_paths){
        return nullptr;
    }

    template< typename TImage>
    TImage*
    PipelineBuilder
    ::padImage(TImage* image, int64_t x, int64_t y){
        return nullptr;
    }

    template< typename TImage>
    TImage*
    PipelineBuilder
    ::flipImage(TImage* image, const std::vector<bool> &flipAxes){
        return nullptr;
    }

    template< typename TImage>
    TImage*
    PipelineBuilder
    ::thresholdImage(TImage* image, float threshold){
        return nullptr;
    }

    template< typename TImage>
    TImage*
    PipelineBuilder
    ::runGraphOnImage(TImage* image, const oxtf::GraphReader *graphReader){
        return nullptr;
    }

    /**
     * TODO: should I make is static or move to Utils?
     * @tparam TImage
     * @param image
     * @param output_dir
     * @return
     */
    template< typename TImage>
    int
    PipelineBuilder
    ::writeImages(TImage *image, const std::string &output_dir){
        return 0; // EXIT_SUCCESS
    }

} // namespace oxtf