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

void train_cnn(network<sequential> &net, std::string folderName);
void test_cnn(network<sequential> &net, std::string folderName);

void convert_image(std::string imagefilename, double scale, int w, int h, std::vector<vec_t>& data);
//void convert_images(const std::string& directory, double scale, int w, int h, std::vector<vec_t>& data);
void saveVector(std::vector<std::vector<int>> &vec);