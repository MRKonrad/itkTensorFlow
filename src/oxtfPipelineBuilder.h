//
// Created by Konrad Werys on 04/04/2019.
//

#ifndef ITKTENSORFLOW_OXTF_PIPELINEBUILDER_H
#define ITKTENSORFLOW_OXTF_PIPELINEBUILDER_H

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFileTools.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkTileImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkConstantPadImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkFlipImageFilter.h"
#include "itkThresholdImageFilter.h"

#include "itkTensorFlowFilter.h"
#include "oxtfImageToTensor.h"
#include "oxtfTensorToItkImage.h"
#include "oxtfGraphReader.h"
#include "oxtfGraphRunner.h"

#include <string>

namespace oxtf {

    /**
     * TODO: move to seperate folder app?
     */
    class PipelineBuilder {

    public:

        /**
         * the most important method of the class
         * @return
         */
        int runPipeline ();

        PipelineBuilder();

        ~PipelineBuilder() = default;

    private:
        std::vector<std::string> _input_images_grayscale_paths;
        std::string _input_image_rgb_path;

        std::string _output_dir_path;
        std::string _graph_path;

        bool _doPadding;
        std::vector<bool> _flipAxes;
        float _threshold;

        /**
         *
         * @param graph_path
         * @return
         */
        oxtf::GraphReader* graphReaderMaker(const std::string &graph_path);

        /**
         *
         * @tparam TImage
         * @return
         */
        template< typename TImage>
        TImage* readInputImage();

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image_path
         * @return
         */
        template< typename TImage>
        TImage* readInputImageRgb(const std::string &image_path);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param images_paths
         * @return
         */
        template< typename TImage>
        TImage* readInputImageGray(const std::vector<std::string> &images_paths);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image
         * @param x
         * @param y
         * @return
         */
        template< typename TImage>
        TImage* padImage(TImage* image, int64_t x, int64_t y);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image
         * @param flipAxes
         * @return
         */
        template< typename TImage>
        TImage* flipImage(TImage* image, const std::vector<bool> &flipAxes);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image
         * @param threshold
         * @return
         */
        template< typename TImage>
        TImage* thresholdImage(TImage* image, float threshold);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image
         * @param graphReader
         * @return
         */
        template< typename TImage>
        TImage* runGraphOnImage(TImage* image, const oxtf::GraphReader *graphReader);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image
         * @param output_dir
         * @return
         */
        template< typename TImage>
        int writeImages(TImage *image, const std::string &output_dir);

    };

} // namespace oxtf

#endif //ITKTENSORFLOW_OXTF_PIPELINEBUILDER_H
