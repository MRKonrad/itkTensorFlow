//
// Created by Konrad Werys on 04/04/2019.
//

#include "oxtfPipelineBuilder.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFileTools.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkTileImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkConstantPadImageFilter.h"
#include "itkCropImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkFlipImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkRGBPixel.h"

#include "itkTensorFlowFilter.h"
#include "oxtfImageToTensor.h"
#include "oxtfTensorToItkImage.h"
#include "oxtfGraphRunner.h"

namespace oxtf {

    PipelineBuilder
    ::PipelineBuilder(){
        _paddingOrNor = true;
        _flipAxes = std::vector<bool>(3, false);
        _threshold = NAN;
        _multiplyOutputByFactor = 255;
    };

	PipelineBuilder
	::~PipelineBuilder() {
		//std::cout << "~PipelineBuilder()" << std::endl;
	};

    void
    PipelineBuilder
    ::disp(){

        std::cout << "PipelineBuilder" << std::endl;
        std::cout << "inputImagesGrayscalePaths:" << std::endl;
        for (int i = 0; i < _inputImagesGrayscalePaths.size(); i++){
            std::cout << "\t" << _inputImagesGrayscalePaths[i] << std::endl;
        }

        std::cout << "inputImageRgbPath:\n\t" << _inputImageRgbPath << std::endl;
        std::cout << "graphPath:\n\t" << _graphPath << std::endl;
        std::cout << "outputDirPath:\n\t" << _outputDirPath << std::endl;
        std::cout << "padding: " << (_paddingOrNor? "Yes" : "No") << std::endl;

        for (int i = 0; i < _flipAxes.size(); i++){
            std::cout << "_flipAxes[" << i << "]: " << (_flipAxes[i] ? "Yes" : "No") << std::endl;
        }

        std::cout << "threshold: " << _threshold << std::endl;
        std::cout << "multiplyOutputByFactor: " << _multiplyOutputByFactor << std::endl;
        std::cout << std::endl;

    }

    int
    PipelineBuilder
    ::runPipeline (){

        typedef float PixelTypeIn;
        typedef itk::Image<PixelTypeIn, 3> ImageType;
        typename ImageType::SizeType size;
        oxtf::GraphReader *graphReader = graphReaderMaker(_graphPath);
        if (!graphReader)
            return 2; // EXIT_FAILURE

        graphReader->disp();

        typename ImageType::Pointer imageIn = readInputImage<ImageType>();

        if (!imageIn)
            return 1; // EXIT_FAILURE

        if (_paddingOrNor) {
            size = imageIn->GetLargestPossibleRegion().GetSize();
            imageIn = padImage<ImageType>(imageIn, graphReader->getMaxX(), graphReader->getMaxY());
        }

        if (_flipAxes[0] || _flipAxes[1] || _flipAxes[2]){
            imageIn = flipImage<ImageType>(imageIn, _flipAxes);
        }

        if (!isnan(_threshold)){
            imageIn = thresholdImage<ImageType>(imageIn, _threshold);
        }

        typename ImageType::Pointer imageOut = runGraphOnImage<ImageType>(imageIn, graphReader);

        if (imageOut.IsNull()){
            delete graphReader;
            return 1; // EXIT_FAILURE
        }

//        if (_flipAxes[0] || _flipAxes[1] || _flipAxes[2]){
//            imageOut = flipImage<ImageType>(imageOut, _flipAxes);
//        }
//
//        if (_paddingOrNor) {
//            imageOut = cropImage<ImageType>(imageOut, size[0], size[1]);
//        }
//
        imageOut = multiplyImage<ImageType>(imageOut, _multiplyOutputByFactor);

        writeImages<ImageType>(imageOut, _outputDirPath);

        delete graphReader;

        return 0; // EXIT_SUCCESS
    }

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

        if (_inputImagesGrayscalePaths.empty() && _inputImageRgbPath.empty()){
            std::cerr << "Provide inputImagesGrayscalePaths OR inputImageRgbPath" << std::endl;
            return nullptr;
        }
        if (!_inputImagesGrayscalePaths.empty() && !_inputImageRgbPath.empty()){
            std::cerr << "Provide inputImagesGrayscalePaths OR inputImageRgbPath, not both" << std::endl;
            return nullptr;
        }

        if        (!_inputImagesGrayscalePaths.empty()) {
            return readInputImageGray<TImage>(_inputImagesGrayscalePaths);
        } else if (!_inputImageRgbPath.empty()){
            return readInputImageRgb<TImage>(_inputImageRgbPath);
        }

        return nullptr;
    }

    template< typename TImage>
    typename TImage::Pointer
    PipelineBuilder
    ::readInputImageRgb(const std::string &image_path){

        typedef itk::RGBPixel<std::uint8_t> RgbPixelType;
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
    ::padImage(typename TImage::Pointer image, int64_t x, int64_t y){

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
    ::flipImage(typename TImage::Pointer image, const std::vector<bool> &flipAxes){

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
    ::thresholdImage(typename TImage::Pointer image, float threshold){

        if (isnan(threshold)){
            return image;
        }
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
    ::runGraphOnImage(typename TImage::Pointer imageIn, const oxtf::GraphReader *graphReader){

        TF_Tensor *inputTensor;

        oxtf::ImageToTensor<TImage>::convertWithCasting(imageIn, graphReader->getInputOperationType(), &inputTensor);

        oxtf::GraphRunner graphRunner;
        graphRunner.setGraphReader(graphReader);
        graphRunner.setInputTensor(inputTensor);

        if(graphRunner.run() != 0) {
            return nullptr; // EXIT_FAILURE
        }

        TF_Tensor *outputTensor = graphRunner.getOutputTensor();

        typename TImage::Pointer imageOut = TImage::New();
        oxtf::TensorToImage<TImage>::convert(outputTensor, imageOut);

        return imageOut;
    }

    template< typename TImage>
    typename TImage::Pointer
    PipelineBuilder
    ::cropImage(typename TImage::Pointer image, unsigned long x, unsigned long y){

        typename TImage::SizeType imageSize = image->GetLargestPossibleRegion().GetSize();

        typename TImage::SizeType lowerCropRegion;
        lowerCropRegion[0] = 0;
        lowerCropRegion[1] = 0;
        lowerCropRegion[2] = 0;

        typename TImage::SizeType upperCropRegion;
        upperCropRegion[0] = imageSize[0] - x;
        upperCropRegion[1] = imageSize[1] - y;
        upperCropRegion[2] = 0;

        typedef itk::CropImageFilter<TImage, TImage> CropImageFilterType;
        typename CropImageFilterType::Pointer cropImageFilter = CropImageFilterType::New();
        cropImageFilter->SetInput(image);
        cropImageFilter->SetLowerBoundaryCropSize(lowerCropRegion);
        cropImageFilter->SetUpperBoundaryCropSize(upperCropRegion);
        cropImageFilter->Update();

        return cropImageFilter->GetOutput();
    }

    template< typename TImage>
    typename TImage::Pointer
    PipelineBuilder
    ::multiplyImage(typename TImage::Pointer image, float factor){

        if (factor == 1){
            return image;
        }

        typedef itk::MultiplyImageFilter<TImage> MultiplierType;
        typename MultiplierType::Pointer multiplier = MultiplierType::New();
        multiplier->SetInput(image);
        multiplier->SetConstant(factor);
        multiplier->Update();

        return multiplier->GetOutput();
    }

    template< typename TImage>
    int
    PipelineBuilder
    ::writeImages(TImage *image, const std::string &output_dir){

        typedef typename TImage::PixelType PixelType;
        typedef itk::Image<unsigned char, 2> TImageOut;

        typedef itk::ExtractImageFilter<TImage, TImageOut> ExtractImageFilterType;
        typename ExtractImageFilterType::Pointer extractor = ExtractImageFilterType::New();
        extractor->SetInput(image);
        extractor->SetDirectionCollapseToIdentity();

        typedef itk::ImageRegion< 3 > RegionType3d;
        RegionType3d region3d = image->GetLargestPossibleRegion();
        RegionType3d::SizeType size = region3d.GetSize();
        RegionType3d::IndexType index = region3d.GetIndex();

        itk::FileTools::CreateDirectory(output_dir);
        unsigned long nImages = image->GetLargestPossibleRegion().GetSize()[2];

        for (unsigned int i = 0; i < nImages; i++) {

            size[2] = 0;
            index[2] = i;
            region3d.SetSize(size);
            region3d.SetIndex(index);
            extractor->SetExtractionRegion(region3d);
            typename TImageOut::Pointer extractedImage = extractor->GetOutput();
            extractor->Update();
            extractedImage->DisconnectPipeline();

            typedef itk::ImageFileWriter<TImageOut> WriterType;
            WriterType::Pointer writer = WriterType::New();
            writer->SetFileName(output_dir + fileSeparator() + "image_" + std::to_string(i) + ".png");
            writer->SetInput(extractedImage);
            writer->Update();
        }

        return 0; // EXIT_SUCCESS
    }

    const std::vector<std::string> &PipelineBuilder::getInputImagesGrayscalePaths() const {
        return _inputImagesGrayscalePaths;
    }

    void PipelineBuilder::setInputImagesGrayscalePaths(const std::vector<std::string> &_inputImagesGrayscalePaths) {
        PipelineBuilder::_inputImagesGrayscalePaths = _inputImagesGrayscalePaths;
    }

    const std::string &PipelineBuilder::getInputImageRgbPath() const {
        return _inputImageRgbPath;
    }

    void PipelineBuilder::setInputImageRgbPath(const std::string &_inputImageRgbPath) {
        PipelineBuilder::_inputImageRgbPath = _inputImageRgbPath;
    }

    const std::string &PipelineBuilder::getOutputDirPath() const {
        return _outputDirPath;
    }

    void PipelineBuilder::setOutputDirPath(const std::string &_outputDirPath) {
        PipelineBuilder::_outputDirPath = _outputDirPath;
    }

    const std::string &PipelineBuilder::getGraphPath() const {
        return _graphPath;
    }

    void PipelineBuilder::setGraphPath(const std::string &_graphPath) {
        PipelineBuilder::_graphPath = _graphPath;
    }

    bool PipelineBuilder::isPaddingOrNor() const {
        return _paddingOrNor;
    }

    void PipelineBuilder::setPaddingOrNot(bool _doPadding) {
        PipelineBuilder::_paddingOrNor = _doPadding;
    }

    const std::vector<bool> &PipelineBuilder::getFlipAxes() const {
        return _flipAxes;
    }

    void PipelineBuilder::setFlipAxes(const std::vector<bool> &_flipAxes) {
        PipelineBuilder::_flipAxes = _flipAxes;
    }

    float PipelineBuilder::getThreshold() const {
        return _threshold;
    }

    void PipelineBuilder::setThreshold(float _threshold) {
        PipelineBuilder::_threshold = _threshold;
    }

    float PipelineBuilder::getMultiplyOutputByFactor() const {
        return _multiplyOutputByFactor;
    }

    void PipelineBuilder::setMultiplyOutputByFactor(float _multiplyOutputByFactor) {
        PipelineBuilder::_multiplyOutputByFactor = _multiplyOutputByFactor;
    }

} // namespace oxtf