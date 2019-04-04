//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX
#define ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageAlgorithm.h"
#include "itkTensorFlowFilter.h"
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
    ::VerifyTypesAndSizes() {

        return EXIT_SUCCESS;
    };

    template< class TImageIn, class TImageOut>
    void TensorFlowImageFilter< TImageIn, TImageOut >
    ::GenerateData() {

        VerifyTypesAndSizes();

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
