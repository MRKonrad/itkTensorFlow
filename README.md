# itkTensorFlow

heavily based on https://github.com/Neargye/hello_tf_c_api

# How to see a tensorboard from a model
```bash
python ~/Code/tensorflow/tensorflow/python/tools/import_pb_to_tensorboard.py --model_dir tests/testData/model.pb --log_dir tests/testData/log
tensorboard --logdir tests/testData/log
```

# Models
https://github.com/tensorflow/models/blob/master/research/deeplab/g3doc/model_zoo.md
https://colab.research.google.com/github/tensorflow/models/blob/master/research/deeplab/deeplab_demo.ipynb