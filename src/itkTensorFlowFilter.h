//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILER_H
#define ITKTENSORFLOW_ITKTENSORFLOWFILER_H

#include "itkImageToImageFilter.h"
#include "oxtfUtils.h"

namespace itk
{

//    template<
//            typename TImageIn = itk::Image< unsigned char, 2 >,
//            typename TImageInVec = itk::Image< itk::RGBPixel< unsigned char >, 2 >,
//            typename TImageOut = itk::Image< unsigned char, 2 >,
//            typename TImageOutVec = itk::Image< itk::RGBPixel< unsigned char >, 2 >,
//    >
    template< typename TImageIn, typename TImageOut>
    class TensorFlowImageFilter:public ImageToImageFilter< TImageIn, TImageOut >
    {
    public:
        /** Standard class typedefs. */
        typedef TensorFlowImageFilter Self;
        typedef ImageToImageFilter< TImageIn, TImageOut > Superclass;
        typedef SmartPointer< Self > Pointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self);

        /** Run-time type information (and related methods). */
        itkTypeMacro(ImageFilter, ImageToImageFilter);

        itkSetMacro( ModelPath, std::string );
        itkGetMacro( ModelPath, std::string );


    protected:
        TensorFlowImageFilter();
        ~TensorFlowImageFilter(){}

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;

    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(TensorFlowImageFilter); //purposely not implemented

        std::string m_ModelPath;
    };
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorFlowFilter.hxx"
#endif

#endif //ITKTENSORFLOW_ITKTENSORFLOWFILER_H
