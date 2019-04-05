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
    typename TImage::Pointer
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
    typename TImage::Pointer
    PipelineBuilder
    ::readInputImageRgb(const std::string &image_path){

        typedef itk::RGBPixel<unsigned char> RgbPixelType;
        typedef itk::Image<RgbPixelType, 2> RgbImageType;

        typedef itk::ImageFileReader<RgbImageType> ReaderType;
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName(image_path);
        reader->Update();

        typedef itk::VectorIndexSelectionCastImageFilter<RgbImageType, TImage> ExtractFromVectorFilterType;
        typename ExtractFromVectorFilterType::Pointer extractFromVectorFilter = ExtractFromVectorFilterType::New();
        extractFromVectorFilter->SetInput(reader->GetOutput());

        typedef itk::TileImageFilter<TImage, TImage> TileImageFilterType;
        typename TileImageFilterType::Pointer tiler = TileImageFilterType::New();
        itk::FixedArray<unsigned int, 3> layout;
        layout[0] = 1;
        layout[1] = 1;
        layout[2] = 0;
        tiler->SetLayout(layout);

        RgbPixelType tempPixel;
        for (unsigned int i = 0; i < tempPixel.Size(); i++) {
            extractFromVectorFilter->SetIndex(i);
            extractFromVectorFilter->Update();
            typename TImage::Pointer input = extractFromVectorFilter->GetOutput();
            input->DisconnectPipeline();
            tiler->SetInput(i, input);
        }
        tiler->Update();
        return tiler->GetOutput();
    }

    template< typename TImage>
    typename TImage::Pointer
    PipelineBuilder
    ::readInputImageGray(const std::vector<std::string> &images_paths){

        typedef typename TImage::PixelType PixelType;
        typedef itk::Image<PixelType, 2> TImage2d;

        typedef itk::ImageFileReader<TImage2d> ReaderType;
        typename ReaderType::Pointer reader = ReaderType::New();

        typedef itk::TileImageFilter<TImage2d, TImage> TileImageFilterType;
        typename TileImageFilterType::Pointer tiler = TileImageFilterType::New();
        itk::FixedArray<unsigned int, 3> layout;
        layout[0] = 1;
        layout[1] = 1;
        layout[2] = 0;
        tiler->SetLayout(layout);

        for (unsigned int i = 0; i < images_paths.size(); ++i) {

            reader->SetFileName(images_paths[i]);
            reader->Update();
            typename TImage2d::Pointer input = reader->GetOutput();
            input->DisconnectPipeline();
            tiler->SetInput(i, input);
        }
        tiler->Update();
        return tiler->GetOutput();
    }

    template< typename TImage>
    typename TImage::Pointer
    PipelineBuilder
    ::padImage(TImage* image, int64_t x, int64_t y){

        typedef itk::ImageRegion< 3 > RegionType;
        RegionType region = image->GetLargestPossibleRegion();

        if (x < region.GetSize()[0] || y < region.GetSize()[1]){
            throw std::runtime_error("padImage error ");
        }

        typename TImage::SizeType lowerExtendRegion;
        lowerExtendRegion[0] = 0;
        lowerExtendRegion[1] = 0;
        lowerExtendRegion[2] = 0;

        typename TImage::SizeType upperExtendRegion;
        upperExtendRegion[0] = x - region.GetSize()[0];
        upperExtendRegion[1] = y - region.GetSize()[1];
        upperExtendRegion[2] = 0;

        typename TImage::PixelType zeroPixel(0);

        typedef itk::ConstantPadImageFilter< TImage, TImage > ConstantPadType;
        typename ConstantPadType::Pointer constantPad = ConstantPadType::New();
        constantPad->SetPadLowerBound(lowerExtendRegion);
        constantPad->SetPadUpperBound(upperExtendRegion);
        constantPad->SetConstant(zeroPixel);
        constantPad->SetInput(image);
        constantPad->Update();

        return constantPad->GetOutput();
    }

    template< typename TImage>
    typename TImage::Pointer
    PipelineBuilder
    ::flipImage(TImage* image, const std::vector<bool> &flipAxes){

        itk::FixedArray<bool, 3> localflipAxes;
        localflipAxes[0] = flipAxes[0];
        localflipAxes[1] = flipAxes[1];
        localflipAxes[2] = flipAxes[2];

        typedef itk::FlipImageFilter <TImage> FlipImageFilterType;
        typename FlipImageFilterType::Pointer flipFilter = FlipImageFilterType::New ();
        flipFilter->SetInput(image);
        flipFilter->SetFlipAxes(localflipAxes);
        flipFilter->Update();

        return flipFilter->GetOutput();
    }

    template< typename TImage>
    typename TImage::Pointer
    PipelineBuilder
    ::thresholdImage(TImage* image, float threshold){

        typedef itk::ThresholdImageFilter <TImage> ThresholdImageFilterType;
        typename ThresholdImageFilterType::Pointer thresholdFilter = ThresholdImageFilterType::New();
        thresholdFilter->SetInput(image);
        thresholdFilter->ThresholdAbove(threshold);
        thresholdFilter->SetOutsideValue(threshold);
        thresholdFilter->Update();

        return thresholdFilter->GetOutput();
    }

    template< typename TImage>
    typename TImage::Pointer
    PipelineBuilder
    ::runGraphOnImage(TImage* imageIn, const oxtf::GraphReader *graphReader){

        TF_Tensor *inputTensor;
        oxtf::ImageToTensor<TImage>(imageIn, &inputTensor);

        oxtf::GraphRunner graphRunner;
        graphRunner.setGraphReader(graphReader);
        graphRunner.setInputTensor(inputTensor);

        graphRunner.run();

        TF_Tensor *outputTensor = graphRunner.getOutputTensor();

        typename TImage::Pointer imageOut = TImage::New();
        oxtf::TensorToImage<TImage>(outputTensor, imageOut);

        return nullptr;
    }

    template< typename TImage>
    int
    PipelineBuilder
    ::writeImages(TImage *image, const std::string &output_dir){
        return 0; // EXIT_SUCCESS
    }

} // namespace oxtf