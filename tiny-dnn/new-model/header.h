#include "tiny_dnn/tiny_dnn.h"
#include <opencv2/opencv.hpp>

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;

network<sequential> construct_cnn();
void train_cnn(network<sequential> &net);
void test_dnn(network<sequential> &net);

void convert_image(std::string imagefilename, double scale, int w, int h, std::vector<vec_t>& data);
//void convert_images(const std::string& directory, double scale, int w, int h, std::vector<vec_t>& data);