//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX
#define ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageAlgorithm.h"
#include "itkTensorFlowFilter_test.h"
#include "oxtfImageToTensor.h"
#include "oxtfTensorToItkImage.h"
#include "oxtfGraphRunner.h"

namespace itk {

    template< class TImageIn, class TImageOut>
    TensorFlowImageFilter< TImageIn, TImageOut >
    ::TensorFlowImageFilter() {
        this->SetNumberOfRequiredInputs(1);
        this->SetNumberOfRequiredOutputs(1);
    }

    template<typename TImageIn, typename TImageOut>
    void
    TensorFlowImageFilter<TImageIn, TImageOut>
    ::GenerateOutputInformation(){
        typename TImageIn::ConstPointer inputImage = this->GetInput(0);
        typename TImageIn::RegionType inputRegion = inputImage->GetLargestPossibleRegion();

        typename TImageOut::IndexType start;
        typename TImageOut::SizeType size;
        typename TImageOut::SpacingType spacing;
        typename TImageOut::PointType origin;
        typename TImageOut::DirectionType direction;

        start.Fill(0);
        size.Fill(0);
        spacing.Fill(1);
        origin.Fill(0);
        direction.SetIdentity();

        int inNumDims = inputRegion.GetSize().GetSizeDimension();
        int outNumDims = size.GetSizeDimension();
        int numDims = std::min(inNumDims, outNumDims);

        // try copying
        for (int i = 0; i < numDims; ++i){
            start[i] = inputRegion.GetIndex()[i];
            size[i] = inputRegion.GetSize()[i];
            spacing[i] = inputImage->GetSpacing()[i];
            origin[i] = inputImage->GetOrigin()[i];
            for (int j = 0; j < numDims; ++j){
                direction[i][j] = inputImage->GetDirection()[i][j];
            }
        }

        typename TImageOut::RegionType region;
        region.SetSize(size);
        region.SetIndex(start);

        for (unsigned int i = 0; i < this->GetNumberOfOutputs(); i++){
            this->GetOutput(i)->SetLargestPossibleRegion(region);
            this->GetOutput(i)->SetSpacing(spacing);
            this->GetOutput(i)->SetOrigin(origin);
            this->GetOutput(i)->SetDirection(direction);
        }
    }

    template< class TImageIn, class TImageOut>
    int TensorFlowImageFilter< TImageIn, TImageOut >
    ::VerifySizes() {

        typename TImageIn::SizeType inputSize = this->GetInput(0)->GetLargestPossibleRegion().GetSize();

        int inputImageNumDims = inputSize.GetSizeDimension();
        int graphInputDims = (int)this->m_GraphReader->getInputOperationDims();
        if (inputImageNumDims != graphInputDims-1){
            std::cerr << "inputImageNumDims: " << inputImageNumDims << " graphInputDims: " << graphInputDims <<
                      ". graphInputDims should be 1 more than graphInputDims " << std::endl;
            throw std::runtime_error("Number of dimensions is different in the graph input and input image ");
        }

        int outputImageNumDims = this->GetOutput(0)->GetLargestPossibleRegion().GetSize().GetSizeDimension();
        int graphOutputDims = (int)this->m_GraphReader->getOutputOperationDims();
        if (outputImageNumDims != graphOutputDims-1){
            std::cerr << "outputImageNumDims: " << outputImageNumDims << " graphOutputDims: " << graphOutputDims <<
                      ". outputImageNumDims should be 1 more than graphOutputDims " << std::endl;
            throw std::runtime_error("Number of dimensions is different in the graph output and output image ");
        }

        std::vector<std::int64_t> biggestTensor2nd3rd = this->m_GraphReader->getOperationWithBiggest2nd3rdSize();
        if (inputSize[0] > biggestTensor2nd3rd[1] || inputSize[1] > biggestTensor2nd3rd[2]){
            std::cerr << "Image is bigger than the biggest tensor in the graph.\nImage: [" <<
                      inputSize[0] << " " << inputSize[1] << "] biggest tensor: [" << biggestTensor2nd3rd[1] <<
                      " " << biggestTensor2nd3rd[2] << "] " << std::endl;
            throw std::runtime_error("Image is bigger than the biggest tensor in the graph ");
        }

        return EXIT_SUCCESS;
    };

    template< class TImageIn, class TImageOut>
    void TensorFlowImageFilter< TImageIn, TImageOut >
    ::GenerateData() {

        VerifySizes();

        TF_Tensor *inputTensor;
        if( oxtf::ImageToTensor<TImageIn>(this->GetInput(), &inputTensor, 4) != EXIT_SUCCESS ){
            throw std::runtime_error("Error in oxtf::ImageToTensor");
        }

        oxtf::GraphRunner graphRunner;
        graphRunner.setGraphReader(this->m_GraphReader);
        graphRunner.setInputTensor(inputTensor);

        if( graphRunner.run() != EXIT_SUCCESS ){
            throw std::runtime_error("Error in oxtf::GraphRunner.run()");
        }

        TF_Tensor *outputTensor = graphRunner.getOutputTensor();

        typename TImageOut::Pointer image = TImageOut::New();
        if( oxtf::TensorToImage<TImageOut>(outputTensor, this->GetOutput()) != EXIT_SUCCESS ){
            throw std::runtime_error("Error in oxtf::TensorToImage");
        }

        tf_utils::DeleteTensor(inputTensor);
        tf_utils::DeleteTensor(outputTensor);
    }

}// end namespace itk

#endif //ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX
