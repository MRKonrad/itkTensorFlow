# itkTensorFlow
ITK pipeline using provided tensorflow .pd model.  
Heavily based on [Neargye/hello_tf_c_api](https://github.com/Neargye/hello_tf_c_api)

| System   |      Badge      |
|----------|:---------------:|
| Travis OSX/Linux build | [![Build Status](https://travis-ci.org/MRKonrad/itkTensorFlow.svg?branch=master)](https://travis-ci.org/MRKonrad/itkTensorFlow)| 


# How to use
OSX/Linux
```console
git clone https://github.com/MRKonrad/itkTensorFlow.git
cd itkTensorflow
git submodule update --init --recursive
mkdir bin
cd bin
cmake ..
cmake --build .
```

# How to see a tensorboard from a model
```console
python ~/Code/tensorflow/tensorflow/python/tools/import_pb_to_tensorboard.py --model_dir tests/testData/model.pb --log_dir tests/testData/log
tensorboard --logdir tests/testData/log
```

# Models
The sample model used in tests can be found here:
https://github.com/tensorflow/models/blob/master/research/deeplab/g3doc/model_zoo.md
https://colab.research.google.com/github/tensorflow/models/blob/master/research/deeplab/deeplab_demo.ipynb

# Thoughts

## How to provide n 2d input images
* itkImage with third dimension as the n-th image
**Pro:** can be defined in the runtime
**Con:** breaches the logic behind itk image
* itkImage with n-dim vector type 
**Pro:** makes sense with itk logic  
**Con:** have to be defined at the compile time
```c
PixelType = itk::Vector< float, n >; // n has to be static, cannot be defined in the runtime
ImageType = itk::Image< PixelType, 2 >;
```
* n inputs to a itkImageFilter class  
**Pro:** makes sense with itk logic  
**Con:** number of itkImageFilter have to be defined in a constructor. itkImageFilter constructor does not accept arguments. Hence the n has to be defined in the compile time  
**Partial solution:** allow big number of inputs 

## Other
* colormap in rgb: https://itk.org/Doxygen/html/classitk_1_1ScalarToRGBColormapImageFilter.html