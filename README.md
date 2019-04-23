# itkTensorFlow
ITK pipeline using provided tensorflow .pd model.  
Heavily based on [Neargye/hello_tf_c_api](https://github.com/Neargye/hello_tf_c_api)
Feel free to contact me if you need new itkTensorFlow features, for example multiple inputs/outputs.

| System   |      Badge      |
|----------|:---------------:|
| [OSX/Linux build - Travis][travis_link]   | [![Travis (.org)][travis_badge]][travis_link]                      |
| [Windows build - AppVeyor][appveyor_link] | [![AppVeyor][appveyor_badge]][appveyor_link]                       |
| [Downloads][downloads_link]               | [![GitHub Releases (by Release)][downloads_badge]][downloads_link] |

# How to use the executable
You can [download the library and the executable from here.][downloads_link] Options available for the executable execution:
```console
>> itkTensorFlowExe -h
Use: itkTensorFlowExe
	 -r inputRgbFilePath
	 -g inputGrayFilePaths, can be used multiple times
	 -m inputGraphPath tensor flow model/graph (in .pb format)
	 -o outputDirPath
	 -s scale (multiply output values by this value)
	 -h help
```

## Example 1: one rgb image as input
If you want to use **one** rgb image (Windows command line):
```console
itkTensorFlowExe.exe -r "image2.jpg" -m "path/to/model.pb" -o "output"
```
OSX/Linux command line:
```console
./itkTensorFlowExe -r "image2.jpg" -m "path/to/model.pb" -o "output" 
```

## Example 2: several grayscale images as input
If you want to use **one or more** grayscale images (Windows command line):
```console
itkTensorFlowExe.exe ^
                    -g "image0.dcm" ^
                    -g "image1.dcm" ^
                    -g "image2.dcm" ^
                    -m "path/to/model.pb" ^
                    -o "output"
```

OSX/Linux command line:
```console
./itkTensorFlowExe \
                    -g "image0.dcm" \
                    -g "image1.dcm" \
                    -g "image2.dcm" \
                    -m "path/to/model.pb"  \
                    -o "output"
```

You can find examples of using executable here: https://github.com/MRKonrad/itkTensorFlow/tree/master/scripts

# How to compile
OSX/Linux
```console
git clone https://github.com/MRKonrad/itkTensorFlow.git
cd itkTensorflow
git submodule update --init --recursive
cmake . -Bbin -DITK_DIR_HINTS="../ITK_install"
cmake --build bin
```

Windows
```console
git clone https://github.com/MRKonrad/itkTensorFlow.git
cd itkTensorflow
git submodule update --init --recursive
cmake . -Bbin -G"Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX=install -DITK_DIR_HINTS="../ITK_install"
cmake --build bin --config RELSEASE --target INSTALL
```

# Models/Graphs tested
The sample model used in tests can be found in a nice jupyter notebook:
https://colab.research.google.com/github/tensorflow/models/blob/master/research/deeplab/deeplab_demo.ipynb  
In this notebook, to get the image indexing right, I was playing with rotated image:
http://mrkonrad.github.io/MRKonrad/files/cnnRotatedImage/image2_90.jpg  
http://mrkonrad.github.io/MRKonrad/files/cnnRotatedImage/image2_180.jpg  
http://mrkonrad.github.io/MRKonrad/files/cnnRotatedImage/image2_270.jpg  

# Troubleshooting
* missing `msvcp140.dll` 
Download https://www.microsoft.com/en-us/download/details.aspx?id=48145 as suggested in https://stackoverflow.com/questions/32998902/msvcp140-dll-missing 
* how to generate `.pb` (tensorflow) file from `.h5` (keras)   
https://stackoverflow.com/questions/45466020/how-to-export-keras-h5-to-tensorflow-pb    
https://github.com/amir-abdi/keras_to_tensorflow  
```console
python keras_to_tensorflow.py 
       --input_model="path/to/keras/model.h5" 
       --output_model="path/to/save/model.pb"
```
* How to see model in a `tensorboard`
```console
python ~/Code/tensorflow/tensorflow/python/tools/import_pb_to_tensorboard.py --model_dir tests/testData/model.pb --log_dir tests/testData/log
tensorboard --logdir tests/testData/log
```

# TODO:
* remove dependency to hello_tf_c_api

# Thoughts

## Reason for using 3d itk input images
These are the ways to  provide n 2d input images:
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

In this project I went with the first option.

## Git and conventional commits
I am trying to follow the convetional commits guide:
* https://www.conventionalcommits.org/en/v1.0.0-beta.2/
* https://www.cheatography.com/albelop/cheat-sheets/conventional-commits/

## Other
* colormap in rgb: https://itk.org/Doxygen/html/classitk_1_1ScalarToRGBColormapImageFilter.html

[travis_badge]: https://img.shields.io/travis/MRKonrad/itkTensorFlow/master.svg?style=flat-square
[travis_link]: https://travis-ci.org/MRKonrad/itkTensorFlow
[appveyor_badge]: https://img.shields.io/appveyor/ci/MRKonrad/itkTensorFlow/master.svg?style=flat-square
[appveyor_link]: https://ci.appveyor.com/project/MRKonrad/itkTensorFlow
[codecov_badge]: https://img.shields.io/codecov/c/github/MRKonrad/itkTensorFlow.svg?style=flat-square
[codecov_link]: https://codecov.io/gh/MRKonrad/itkTensorFlow
[downloads_link]: https://github.com/MRKonrad/itkTensorFlow/releases
[downloads_badge]: https://img.shields.io/github/downloads/MRKonrad/itkTensorFlow/total.svg?style=flat-square
