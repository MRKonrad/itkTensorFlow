//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILER_H
#define ITKTENSORFLOW_ITKTENSORFLOWFILER_H

#include "itkImageToImageFilter.h"
#include "oxtfGraphReader.h"

namespace itk
{

    template< typename TImageIn, typename TImageOut>
    class TensorFlowImageFilter : public ImageToImageFilter< TImageIn, TImageOut >
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

        oxtf::GraphReader *GetGraphReader() const {
            return m_GraphReader;
        }

        void SetGraphReader(oxtf::GraphReader *graphReader) {
            TensorFlowImageFilter::m_GraphReader = graphReader;
        }

    protected:
        TensorFlowImageFilter();
        ~TensorFlowImageFilter(){}

        /** Does the real work. */
        virtual void GenerateData() ITK_OVERRIDE;

        /** handle dimensions difference between 2d input and 3d output */
        virtual void GenerateOutputInformation() ITK_OVERRIDE;

        /** Check if input/output types and sizes are compatible with graph's types and sizes */
        virtual int VerifyTypesAndSizes();

    private:
        ITK_DISALLOW_COPY_AND_ASSIGN(TensorFlowImageFilter); //purposely not implemented

        oxtf::GraphReader *m_GraphReader;

    };
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorFlowFilter.hxx"
#endif

#endif //ITKTENSORFLOW_ITKTENSORFLOWFILER_H
