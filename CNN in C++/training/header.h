#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

#define CNN_USE_OMP
#define CNN_USE_SSE
#include "tiny_dnn/tiny_dnn.h"
#include <opencv2/opencv.hpp>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;

network<sequential> construct_cnn();
network<sequential> construct_large_cnn();
void printInfo(network<sequential> net);

void train_cnn_ubyte(network<sequential> &net, std::string folderName, int idx, float learning_rate, int batch_size, int epochs);
void train_cnn_CV(std::string modelName, std::string folderName, int numOfFold, float learning_rate, int batch_size, int epochs);
void cross_validation(const std::string directory, std::vector<vec_t> &train_images, std::vector<label_t> &train_labels, std::vector<vec_t> &validation_images, std::vector<label_t> &validation_labels);

void test_cnn(network<sequential> &net, std::string folderName);
void test_cnn_CV(network<sequential> &net, std::string folderName, int idx, const std::string name);

int demo(network<sequential> &net, int camIndex);

void saveVector(std::vector<std::vector<int>> &vec, const std::string name);
int getLabelIdx(std::vector<label_t> train_labels, std::vector<size_t> &classIdx);
int parse_ubyte_cv(std::vector<vec_t> train_images, std::vector<label_t> train_labels, std::vector<vec_t> &sub_train_images, std::vector<label_t> &sub_train_labels, std::vector<size_t> classIdx, int numOfFold);
int convert_image(cv::Mat img, vec_t& data);
//void convert_images(const std::string & directory, std::vector<vec_t>& data);
int detectAndCrop(cv::Mat &frame, cv::CascadeClassifier &face_cascade);

//utils.cpp
std::string get_date_string();


const std::vector<std::string> emotion_labels{ "neutral", "anger", "disgust", "fear", "happy", "sadness", "surprise" };