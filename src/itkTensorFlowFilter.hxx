//
// Created by Konrad Werys on 24/03/2019.
//

#ifndef ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX
#define ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageAlgorithm.h"

namespace itk {

    template< class TImageIn, class TImageOut>
    TensorFlowImageFilter< TImageIn, TImageOut >
    ::TensorFlowImageFilter() {
        this->SetNumberOfRequiredInputs(1);
        this->SetNumberOfRequiredOutputs(1);

        for (unsigned i = 0; i < 15; ++i) {
            this->SetNthOutput(i, (TImageOut::New()).GetPointer());
        }
    }

    template< class TImageIn, class TImageOut>
    void TensorFlowImageFilter< TImageIn, TImageOut >
    ::GenerateData() {



        typedef typename TImageIn::PixelType PixelInType;
        typedef typename TImageOut::PixelType PixelOutType;
        typename TImageIn::ConstPointer input = this->GetInput();
        typename TImageOut::Pointer output = this->GetOutput();
        this->AllocateOutputs();

//        const PixelInType *zeroPixel = &(input->GetBufferPointer()[0]);
        //auto PixelInType::ValueType aaa;
//        std::cout << '\n';
//        std::cout << typeid(zeroPixel).name() << '\n';
//        std::cout << zeroPixel << '\n';
//        std::cout << zeroPixel[0] << '\n';
//        std::cout << "input: " << input->GetNumberOfComponentsPerPixel() << std::endl;
//        std::cout << "output: "<< output->GetNumberOfComponentsPerPixel() << std::endl;
//
////        typename TImageIn::PixelType pixelPointer;
//        unsigned int dimension=TImageIn::ImageDimension;
////        std::cout << pixelPointer[0] << '\n';
//        std::cout << dimension << '\n';

//        zeroPixel->SetRed(1);
//        zeroPixel[0] = 1;
//        zeroPixel[1] = 100;
//        zeroPixel[2] = 200;

        using RegionType = itk::ImageRegion<2>;

        RegionType region = input->GetLargestPossibleRegion();

        long    sizeX = region.GetSize(0),
                sizeY = region.GetSize(1),
                sizeZ = 3;
                //sizeZ = input->GetNumberOfComponentsPerPixel();

        const std::vector<std::int64_t> input_dims = {1, sizeX, sizeY, sizeZ};
        std::vector<PixelInType> input_vals(sizeX * sizeY * sizeZ, 0);
//        const std::vector<std::int64_t> input_dims = {1, sizeX, sizeY, 1};
//        std::vector<PixelInType> input_vals(sizeX * sizeY, 1);
//
//        for (int i = 0; i < sizeX * sizeY; ++i){
//            input_vals[i] = input->GetBufferPointer()[i];
//        }

        for (int i = 0; i < sizeX * sizeY; ++i){
            input_vals[i*3+0] = input->GetBufferPointer()[i];
            input_vals[i*3+1] = input->GetBufferPointer()[i];
            input_vals[i*3+2] = input->GetBufferPointer()[i];
        }

//        for (int i = 0; i < sizeX * sizeY; ++i){
//            input_vals[i] = input->GetBufferPointer()[i];
//            input_vals[i+sizeX * sizeY] = input->GetBufferPointer()[i];
//            input_vals[i+2*sizeX * sizeY] = input->GetBufferPointer()[i];
//        }


        TF_Tensor* input_tensor = tf_utils::CreateTensor(m_InputTensorTF_DataType,
                                                         input_dims.data(), input_dims.size(),
                                                         input_vals.data(), input_vals.size() * sizeof(PixelInType));
        TF_Tensor* output_tensor = nullptr;

        sessionRun(input_tensor, output_tensor, m_ModelPath, m_InputNodeName, m_OutputNodeName);

        int NumDims = TF_NumDims(input_tensor);
        std::cout << "output_tensor size: ";
        for (int i = 0; i < NumDims; ++i){
            std::cout << TF_Dim(input_tensor, i) << " ";
        }
        std::cout << TFDataTypeToString(TF_TensorType(input_tensor)) << std::endl;

        NumDims = TF_NumDims(output_tensor);
        std::cout << "output_tensor size: ";
        for (int i = 0; i < NumDims; ++i){
            std::cout << TF_Dim(output_tensor, i) << " ";
        }
        std::cout << TFDataTypeToString(TF_TensorType(output_tensor)) << std::endl;

        std::cout << output_tensor << std::endl;


//        typename TImageIn::IndexType myindex;
//        myindex[0] = 1;
//        myindex[1] = 1;

//        auto b = input->GetBufferPointer()[0];
//        //std::cout << typeid(b).name() << '\n';
//        auto c = input->GetPixel(myindex);
//        std::cout << typeid(c).name() << '\n';
//        if (input->GetNumberOfComponentsPerPixel() > 1){
//            std::cout << c.GetNthComponent(1) << std::endl;
//        }
//        //output->GetPixel(myindex) = c[1];

//        for (int i = 0; i < sizeX*sizeY; i++){
//            itk::FixedArray<unsigned char, 3> a = input->GetBufferPointer()[i];
//            //output->GetBufferPointer()[i] = a[0];
//            auto b = input->GetBufferPointer()[0];
//            //std::cout << typeid(b).name() << '\n';
//            auto c = input->GetPixel(myindex);
//            //std::cout << typeid(b).name() << '\n';
//            //output->GetPixel(myindex) = c[1];
//        }

        const auto data = static_cast<int64_t *>(TF_TensorData(output_tensor));

        for (int i = 0; i < sizeX*sizeY; i++){
            output->GetBufferPointer()[i] = data[i];
            //output->GetBufferPointer()[i] = input_vals[i];
        }

//        PixelType zeroPixel;
//        zeroPixel.SetRed(4);
//        zeroPixel.SetGreen(4);
//        zeroPixel.SetBlue(2);
//
//        for (int i = 0; i < sizeX*sizeY/2; i++){
//            output->GetBufferPointer()[i] = zeroPixel;
//        }

        tf_utils::DeleteTensor(input_tensor);
        tf_utils::DeleteTensor(output_tensor);

    }

}// end namespace itk

#endif //ITKTENSORFLOW_ITKTENSORFLOWFILTER_HXX
