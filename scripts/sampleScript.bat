# Paths to download:
#  image: https://github.com/tensorflow/models/blob/master/research/deeplab/g3doc/img/image2.jpg?raw=true
#  model: http://download.tensorflow.org/models/deeplabv3_mnv2_pascal_train_aug_2018_01_29.tar.gz

itkTensorFlowExe.exe ^
    -r "image2.jpg" ^
    -m "deeplabv3_mnv2_pascal_train_aug/frozen_inference_graph.pb" ^
    -o "output"
