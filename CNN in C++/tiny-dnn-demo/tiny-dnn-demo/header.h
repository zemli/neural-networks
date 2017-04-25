#include "tiny_dnn/tiny_dnn.h"
using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;
using namespace std;

network<sequential> construct_cnn();
void train_cnn(network<sequential> &net);