//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX
#define ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageAlgorithm.h"

namespace itk
{

    template< class TImage>
    void TensorFlowImageFilter< TImage>
    ::GenerateData(){

        typename TImage::ConstPointer input = this->GetInput();
        typename TImage::Pointer output = this->GetOutput();

        itk::Index<2> cornerPixel = input->GetLargestPossibleRegion().GetIndex();
        typename TImage::PixelType newValue = 3;

        this->AllocateOutputs();

        ImageAlgorithm::Copy(input.GetPointer(), output.GetPointer(), output->GetRequestedRegion(),
                             output->GetRequestedRegion() );

        output->SetPixel( cornerPixel, newValue );
    }

}// end namespace

#endif //ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX
