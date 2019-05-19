//
// Created by Konrad Werys on 04/04/2019.
//

#ifndef ITKTENSORFLOW_OXTF_PIPELINEBUILDER_H
#define ITKTENSORFLOW_OXTF_PIPELINEBUILDER_H

#include "itktensorflowlib_export.h"
#include "oxtfGraphReader.h"
#include <string>
#include "interp.h"

namespace oxtf {

    class ITKTENSORFLOWLIB_EXPORT PipelineBuilder {

    public:

        /**
         * the most important method of the class
         * @return
         */
        int runPipeline ();

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
        typename TImage::Pointer
        readInputImage();

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image_path
         * @return
         */
        template< typename TImage>
        typename TImage::Pointer
        readInputImageRgb(const std::string &image_path);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param images_paths
         * @return
         */
        template< typename TImage>
        typename TImage::Pointer
        readInputImageGray(const std::vector<std::string> &images_paths);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image
         * @param x
         * @param y
         * @return
         */
        template< typename TImage>
        typename TImage::Pointer
        padImage(typename TImage::Pointer image, int64_t x, int64_t y);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image
         * @param flipAxes
         * @return
         */
        template< typename TImage>
        typename TImage::Pointer
        flipImage(typename TImage::Pointer image, const std::vector<bool> &flipAxes);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image
         * @param threshold
         * @return
         */
        template< typename TImage>
        typename TImage::Pointer
        thresholdImage(typename TImage::Pointer image, float threshold);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image
         * @param graphReader
         * @return
         */
        template< typename TImage>
        typename TImage::Pointer
        runGraphOnImage(typename TImage::Pointer image, const oxtf::GraphReader *graphReader);

        template< typename TImage>
        typename TImage::Pointer
        cropImage(typename TImage::Pointer image, unsigned long x, unsigned long y);

        /**
         *
         * @tparam TImage
         * @param factor
         * @return
         */
        template< typename TImage>
        typename TImage::Pointer
        multiplyImage(typename TImage::Pointer, float factor);

        /**
         * TODO: should I make is static or move to Utils?
         * @tparam TImage
         * @param image
         * @param output_dir
         * @return
         */
        template< typename TImage>
		int writeImages(TImage *image, const std::string &output_dir);

        PipelineBuilder();

		// no virtual destructor, as otherwise I have windows errors about vf table, for example
		// >itkTensorFlowLibStatic.lib(oxtfPipelineBuilder.obj) : error LNK2019: unresolved external symbol "__declspec(dllimport) const oxtf::PipelineBuilder::`vftable'" (__imp_??_7PipelineBuilder@oxtf@@6B@) referenced in function "public: __cdecl oxtf::PipelineBuilder::PipelineBuilder(void)" (??0PipelineBuilder@oxtf@@QEAA@XZ)
		~PipelineBuilder();

		void disp();

        const std::vector<std::string> &getInputImagesGrayscalePaths() const;

        void setInputImagesGrayscalePaths(const std::vector<std::string> &_inputImagesGrayscalePaths);

        const std::string &getInputImageRgbPath() const;

        void setInputImageRgbPath(const std::string &_inputImageRgbPath);

        const std::string &getOutputDirPath() const;

        void setOutputDirPath(const std::string &_outputDirPath);

        const std::string &getGraphPath() const;

        void setGraphPath(const std::string &_graphPath);

        bool isPaddingOrNor() const;

        void setPaddingOrNot(bool _doPadding);

        const std::vector<bool> &getFlipAxes() const;

        void setFlipAxes(const std::vector<bool> &_flipAxes);

        float getThreshold() const;

        void setThreshold(float _threshold);

        float getMultiplyOutputByFactor() const;

        void setMultiplyOutputByFactor(float _multiplyOutputByFactor);

    private:
        std::vector<std::string> _inputImagesGrayscalePaths;
        std::string _inputImageRgbPath;
        std::string _graphPath;
        std::string _outputDirPath;

        bool _paddingOrNor;
        std::vector<bool> _flipAxes;
        float _threshold;
        float _multiplyOutputByFactor;
    };

} // namespace oxtf

#endif //ITKTENSORFLOW_OXTF_PIPELINEBUILDER_H
