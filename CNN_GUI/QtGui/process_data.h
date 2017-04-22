#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "tiny_dnn/tiny_dnn.h"

//declare functions in process_data.cpp
int convert_image(cv::Mat img, tiny_dnn::vec_t& data);
int detectAndCrop(cv::Mat &frame, cv::CascadeClassifier &face_cascade);
