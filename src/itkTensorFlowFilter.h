//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILER_H
#define ITKTENSORFLOW_ITKTENSORFLOWFILER_H

#include "itkImageToImageFilter.h"
#include "itktfUtils.h"

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

        itkSetMacro( InputNodeName, std::string );
        itkGetMacro( InputNodeName, std::string );

        itkSetMacro( OutputNodeName, std::string );
        itkGetMacro( OutputNodeName, std::string );

        itkSetMacro( InputTensorTF_DataType, TF_DataType );
        itkGetMacro( InputTensorTF_DataType, TF_DataType );


    protected:
        TensorFlowImageFilter();
        ~TensorFlowImageFilter(){}

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;

    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(TensorFlowImageFilter); //purposely not implemented

        std::string m_ModelPath;
        std::string m_InputNodeName;
        std::string m_OutputNodeName;
        TF_DataType m_InputTensorTF_DataType;
    };
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorFlowFilter.hxx"
#endif

#endif //ITKTENSORFLOW_ITKTENSORFLOWFILER_H
