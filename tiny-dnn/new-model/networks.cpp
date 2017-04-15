#include "header.h"

void showInfo(network<sequential> net) {
	for (size_t i = 0; i < net.depth(); i++) {
		std::cout << "#layer:" << i << "\n";
		std::cout << "layer type:" << net[i]->layer_type() << "\n";
		std::cout << "input:" << net[i]->in_data_size() << "(" << net[i]->in_data_shape() << ")\n";
		std::cout << "output:" << net[i]->out_data_size() << "(" << net[i]->out_data_shape() << ")\n";
	}
}

network<sequential> construct_cnn() {
	network<sequential> net;
	net << conv<relu>(48, 48, 3, 1, 32, padding::same)
		<< max_pool<tan_h>(48, 48, 32, 2)
		<< conv<relu>(24, 24, 3, 32, 64, padding::same)
		<< max_pool<tan_h>(24, 24, 64, 2)
		<< conv<relu>(12, 12, 3, 64, 128, padding::same)
		<< max_pool<tan_h>(12, 12, 128, 2)
		<< fc<relu>(6 * 6 * 128, 128)
		<< fc<relu>(128, 256)
		<< fc<softmax>(256, 7);

	for (size_t i = 0; i < net.depth(); i++) {
		std::cout << "#layer:" << i << "\n";
		std::cout << "layer type:" << net[i]->layer_type() << "\n";
		std::cout << "input:" << net[i]->in_data_size() << "(" << net[i]->in_data_shape() << ")\n";
		std::cout << "output:" << net[i]->out_data_size() << "(" << net[i]->out_data_shape() << ")\n";
	}
	net.save("new5-Network");
	return net;
}

void train_cnn(network<sequential> &net, std::string folderName) {

	std::vector<label_t> train_labels;
	std::vector<vec_t> train_images;

	std::cout << "reading images from " + folderName + "..." << std::endl;
	parse_mnist_images("../dataset/" + folderName + "/train-images-idx3-ubyte", &train_images, -1.0, 1.0, 0, 0);
	std::cout << "reading labels from "+ folderName +"..." << std::endl;
	parse_mnist_labels("../dataset/" + folderName + "/train-labels-idx1-ubyte", &train_labels);
	adagrad opt;
	size_t batch_size = 16;
	size_t epochs = 30;	
	std::cout << "training on " << train_images.size() << " images, and " << train_labels.size() << " labels" << std::endl;
	net.train<cross_entropy_multiclass>(opt, train_images, train_labels, batch_size, epochs);
	std::cout << "training finished" << std::endl;
	net.save("trained-new-batch-model");
}

void test_cnn(network<sequential> &net, std::string folderName) {
	std::vector<label_t> test_labels;
	std::vector<vec_t> test_images;
	label_t predicted_label;
	int count = 0, right = 0;
	std::vector<std::vector<int>> vec(7, std::vector<int>(7, 0));

	std::cout << "reading images..." << std::endl;
	parse_mnist_images("../dataset/" + folderName + "/test-images-idx3-ubyte", &test_images, -1.0, 1.0, 0, 0);
	std::cout << "reading labels..." << std::endl;	parse_mnist_labels("../dataset/" + folderName + "/test-labels-idx1-ubyte", &test_labels);
	//use cnn model
	for (size_t i = 0; i < test_images.size(); i++) {
		predicted_label = net.predict_label(test_images[i]);
		std::cout << "predicted_label:" << predicted_label << "   label is:" << test_labels[i] << std::endl;
		if (predicted_label == test_labels[i]) right++;
		vec[test_labels[i]][predicted_label]++;
		//std::cout << "similarity:" << net.predict_max_value(test_images[i]) << std::endl;
		count++;
	}
	saveVector(vec);
	//std::cout << "similarity in total:" << simi << std::endl;
	std::cout << "count in total:" << count << "  label size is: " << test_images.size() << std::endl;
	std::cout << "right in total:" << right << std::endl;

}

//// convert image to vec_t
//void convert_image(std::string imagefilename,
//	double scale,
//	int w,
//	int h,
//	std::vector<vec_t>& data)
//{
//
//}