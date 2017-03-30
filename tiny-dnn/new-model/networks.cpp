#include "header.h"


network<sequential> construct_cnn() {
	network<sequential> net;
	
	net << conv<relu>(224, 224, 5, 1, 32, padding::same) // in:224x224x1, 5x5conv, 32fmaps
		<< max_pool<tan_h>(224, 224, 32, 2) // in:220x220x32, 2x2pooling
		<< conv<relu>(112, 112, 5, 32, 64, padding::same)
		<< max_pool<tan_h>(112, 112, 64, 2)
		<< conv<relu>(56, 56, 3, 64, 128, padding::same)
		<< max_pool<tan_h>(56, 56, 128, 2)
		<< conv<relu>(28, 28, 3, 128, 256, padding::same)
		<< max_pool<tan_h>(28, 28, 256, 2) //out: 14x14x256
		<< fc<relu>(14 * 14 * 256, 256)
		<< fc<relu>(1 * 1 * 256, 128)
		<< fc<softmax>(1 * 1 * 128, 7);
		//<< fully_connected_layer<softmax>(14 * 14 * 6, 10);
	//print each layer's information
	for (size_t i = 0; i < net.depth(); i++) {
		std::cout << "#layer:" << i << "\n";
		std::cout << "layer type:" << net[i]->layer_type() << "\n";
		std::cout << "input:" << net[i]->in_data_size() << "(" << net[i]->in_data_shape() << ")\n";
		std::cout << "output:" << net[i]->out_data_size() << "(" << net[i]->out_data_shape() << ")\n";
	}	net.save("myNetwork");
	return net;
}

void train_cnn(network<sequential> &net){
	std::cout << "load models..." << std::endl;
	std::vector<label_t> train_labels;
	std::vector<vec_t> train_images;
	
	std::string	filename = "E:/CKp_database/classified-image/training/0/0-cropped-S011_001_00000001.png";

	convert_image(filename, 1, 224, 224, train_images);
	train_labels.push_back(0);
	adagrad opt;
	size_t batch_size = 1;
	size_t epochs = 30;
	net.train<mse>(opt, train_images, train_labels, batch_size, epochs);
}

void test_dnn(network<sequential> &net){
	std::vector<label_t> test_labels;
	std::vector<vec_t> test_images;
	std::string	filename = "E:/CKp_database/classified-image/training/0/0-cropped-S011_002_00000001.png";
	convert_image(filename, 1, 224, 224, test_images);
	//use cnn model
	std::cout << "result:" << net.predict_label(test_images[0]) << std::endl;
	std::cout << "similarity:" << net.predict_max_value(test_images[0]) << std::endl;
}

// convert image to vec_t
void convert_image(std::string imagefilename,
	double scale,
	int w,
	int h,
	std::vector<vec_t>& data)
{
	cv::Mat img = cv::imread(imagefilename, 1);
	if (!img.data){// cannot open, or it's not an image
		std::cout << "Could not open or find the image" << std::endl;
		return;
	}
	cv::Mat_<uint8_t> resized;
	cv::resize(img, resized, cv::Size(w, h));
	vec_t d;
	if (img.isContinuous()) {
		d.assign((uint8_t)img.datastart, (uint8_t)img.dataend);
	}
	else {
		for (int i = 0; i < img.rows; ++i) {
			d.insert(d.end(), img.ptr<uchar>(i), img.ptr<uchar>(i)+img.cols);
		}
	}
	data.push_back(d);
}