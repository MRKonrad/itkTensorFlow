//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILER_H
#define ITKTENSORFLOW_ITKTENSORFLOWFILER_H

#include "itkImageToImageFilter.h"

namespace itk
{
    template< class TImage>
    class TensorFlowImageFilter:public ImageToImageFilter< TImage, TImage >
    {
    public:
        /** Standard class typedefs. */
        typedef TensorFlowImageFilter                                                 Self;
        typedef ImageToImageFilter< TImage, TImage > Superclass;
        typedef SmartPointer< Self >                                Pointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(ImageFilter, ImageToImageFilter);

    protected:
        TensorFlowImageFilter(){}
        ~TensorFlowImageFilter(){}

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;

    private:
        TensorFlowImageFilter(const Self &); //purposely not implemented
        void operator=(const Self &);  //purposely not implemented

    };
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorFlowFilter.hxx"
#endif

#endif //ITKTENSORFLOW_ITKTENSORFLOWFILER_H
