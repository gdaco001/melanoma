# Melanoma Project

The goal of this project is to develop a computer vision system for the classification of skin tumors. [OpenCV](http://opencv.org/) is the standard library that is used here for image processing with C++ and Python scripts, while the hardware platform is a Raspberry PI 3.

## Installing OpenCV

There are several tutorials about how to install OpenCV in a linux machine (our case) but to simplify this operation one can just follow [this commands](https://github.com/Bianorz/melanoma/blob/master/command_seq_inst_opencv3.txt).

## Segmentation and Labelling

A typical melanoma image can be seen in Fig. 1    
<p align="center">
  <img src="https://raw.githubusercontent.com/Bianorz/melanoma/master/segImEx/Original_Image.png" width="300">
  <b></b><br>
  Fig. 1: Melanoma
  <br><br>
</p>

The method used to image segmentation was the *Otsu Method*, more information can be found [here](http://www.labbookpages.co.uk/software/imgProc/otsuThreshold.html) and [here](http://docs.opencv.org/3.2.0/d7/d4d/tutorial_py_thresholding.html).


<p align="center">
  <img src="https://raw.githubusercontent.com/Bianorz/melanoma/master/segImEx/Image_after_segmentation_median_filter.png" width="300">
  <b></b><br>
  Fig. 2: Fig 1 after Otsu Method + 3x3 median filter
  <br><br>
</p>


Using the Opencv's function [findContours](http://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=findcontours#findcontours) it's possible to detect all enclosed image contours. To label which contour is the melanoma one, we just calculate the distance between the image center (white circle) and the contour centroid (green circle), the shortest distance will be chosen as the melanoma contour.

<p align="center">
  <img src="https://raw.githubusercontent.com/Bianorz/melanoma/master/segImEx/Detected_Contour_Points.png" width="300">
  <b></b><br>
  Fig. 3: chosen contour
  <br><br>
</p>

Finally we need to separate the chosen contour from the original image. One approach to do that is to create a mask with only white pixels inside the contour, make a [bitwise_and](http://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html#bitwise-and) between the mask and the original image and crop a bouding box around the bitwise result. After this steps we get a image with only the melanoma region.

<p align="center">
  <img src="https://raw.githubusercontent.com/Bianorz/melanoma/master/segImEx/Final_Result.png" width="300">
  <b></b><br>
  Fig. 4: Final Result
  <br><br>
</p>

The code that generate this figures can be found [here](https://github.com/Bianorz/melanoma/blob/master/MelanomaPDI/main.cpp).

<p align="center">
  <img src="https://media.tenor.com/images/778b5c865476947a335e00663254a954/tenor.gif" width="300">
  <b></b><br>
  <br><br>
</p>
