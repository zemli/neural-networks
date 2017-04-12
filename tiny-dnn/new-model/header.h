#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "tiny_dnn/tiny_dnn.h"
#include <opencv2/opencv.hpp>

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;

network<sequential> construct_cnn();
network<sequential> construct_small_cnn();
void showInfo(network<sequential> net);

void train_cnn(network<sequential> &net, std::string folderName);
void test_cnn(network<sequential> &net, std::string folderName);

void saveVector(std::vector<std::vector<int>> &vec);
void convert_image(std::string imagefilename, double scale, int w, int h, std::vector<vec_t>& data);
