#include "header.h"

void printInfo(network<sequential> net) {
	for (size_t i = 0; i < net.depth(); i++) {
		std::cout << "#layer:" << i << "\n";
		std::cout << "layer type:" << net[i]->layer_type() << "\n";
		std::cout << "input:" << net[i]->in_data_size() << "(" << net[i]->in_data_shape() << ")\n";
		std::cout << "output:" << net[i]->out_data_size() << "(" << net[i]->out_data_shape() << ")\n";
	}
}

network<sequential> construct_large_cnn() {
	network<sequential> net;
	//
	//net << conv<relu>(224, 224, 5, 1, 32, padding::same) // in:224x224x1, 5x5conv, 32fmaps
	//	<< max_pool<tan_h>(224, 224, 32, 2) // in:220x220x32, 2x2pooling
	//	<< conv<relu>(112, 112, 5, 32, 64, padding::same)
	//	<< max_pool<tan_h>(112, 112, 64, 2)
	//	<< conv<relu>(56, 56, 3, 64, 128, padding::same)
	//	<< max_pool<tan_h>(56, 56, 128, 2)
	//	<< conv<relu>(28, 28, 3, 128, 256, padding::same)
	//	<< max_pool<tan_h>(28, 28, 256, 2) //out: 14x14x256
	//	<< fc<relu>(14 * 14 * 256, 256)
	//	<< fc<relu>(256, 128)
	//	<< fc<softmax>(128, 7);
	//	//<< fully_connected_layer<softmax>(14 * 14 * 6, 10);

	////print each layer's information
	//printInfo(net);
	//net.save("../model/myNetwork");
	return net;
}

network<sequential> construct_cnn() {
	network<sequential> net;
	net << conv<relu>(48, 48, 3, 1, 32, padding::same)
		<< max_pool<tan_h>(48, 48, 32, 2)
		<< conv<relu>(24, 24, 3, 32, 64, padding::same)
		<< max_pool<tan_h>(24, 24, 64, 2)
		<< conv<relu>(12, 12, 3, 64, 128, padding::same)
		<< max_pool<tan_h>(12, 12, 128, 2)
		<< fc<relu>(6 * 6 * 128, 256)
		<< fc<relu>(256, 128)
		<< fc<softmax>(128, 7);

	for (size_t i = 0; i < net.depth(); i++) {
		std::cout << "#layer:" << i << "\n";
		std::cout << "layer type:" << net[i]->layer_type() << "\n";
		std::cout << "input:" << net[i]->in_data_size() << "(" << net[i]->in_data_shape() << ")\n";
		std::cout << "output:" << net[i]->out_data_size() << "(" << net[i]->out_data_shape() << ")\n";
	}
	net.save("newNetwork");
	return net;
}

void train_cnn_ubyte(network<sequential> &net, std::string folderName, int idx, float learning_rate, int batch_size, int epochs){
	
	std::vector<label_t> train_labels;
	std::vector<vec_t> train_images;
	if (idx >= 0) {
		std::cout << "reading images..." << std::endl;
		parse_mnist_images("../dataset/" + folderName + "/train-images-idx3-ubyte" + std::to_string(idx), &train_images, -1.0, 1.0, 0, 0);
		std::cout << "reading labels..." << std::endl;		parse_mnist_labels("../dataset/" + folderName + "/train-labels-idx1-ubyte" + std::to_string(idx), &train_labels);
	}
	else {
		std::cout << "reading images..." << std::endl;
		parse_mnist_images("../dataset/" + folderName + "/train-images-idx3-ubyte", &train_images, -1.0, 1.0, 0, 0);
		std::cout << "reading labels..." << std::endl;		parse_mnist_labels("../dataset/" + folderName + "/train-labels-idx1-ubyte", &train_labels);
	}
	
	adagrad opt;
	opt.alpha = learning_rate;

	std::cout << "training starting..." << std::endl;
	std::cout << "training set has " << train_images.size() << " images and " << train_labels.size() << " labels" << std::endl;
	progress_display disp(train_images.size());
	net.train<cross_entropy_multiclass>(opt, train_images, train_labels, batch_size, epochs);
	std::cout << "training finished" << std::endl;
	//net.save("trained-model");
}

void train_cnn_CV(std::string modelName, std::string folderName, int numOfFold, float learning_rate, int batch_size, int epochs) {		for (int idx = 1; idx <= numOfFold; idx++) {			network<sequential> net;
			std::cout << "loading network: " << modelName << std::endl;
			net.load("../model/" + modelName);			std::vector<label_t> train_labels;
			std::vector<vec_t> train_images;			std::string name = "_" + folderName				+ "_" + std::to_string(learning_rate)				+ "_" + std::to_string(batch_size)				+ "_" + std::to_string(epochs)				+ "_" + std::to_string(idx);			//cross_validation("../dataset/" + folderName, 
			//	train_images, train_labels, 
			//	validation_images, validation_labels);

			train_cnn_ubyte(net, folderName, idx, learning_rate, batch_size, epochs);
			net.save("trained-model" + name);

			test_cnn_CV(net, folderName, idx, name);
		}	
}

void test_cnn(network<sequential> &net, std::string folderName){
	std::vector<label_t> test_labels;
	std::vector<vec_t> test_images;	
	label_t predicted_label;
	
	int right=0;
	std::vector<std::vector<int>> vec(7, std::vector<int>(7, 0));

	std::cout << "reading images..." << std::endl;
	parse_mnist_images("../dataset/"+ folderName +"/test-images-idx3-ubyte", &test_images, -1.0, 1.0, 0, 0);
	std::cout << "reading labels..." << std::endl;	parse_mnist_labels("../dataset/" + folderName + "/test-labels-idx1-ubyte", &test_labels);
	//use cnn model
	clock_t begin = clock();
	for (size_t i = 0; i < test_images.size(); i ++) {
		predicted_label = net.predict_label(test_images[i]);
		std::cout << "predicted_label:" << emotion_labels[predicted_label] 
				  << "   label is:" << emotion_labels[test_labels[i]] 
				  << std::endl;
		if (predicted_label == test_labels[i]) right++;
		vec[test_labels[i]][predicted_label]++;
		//std::cout << "similarity:" << net.predict_max_value(test_images[i]) << std::endl;
	}
	clock_t end = clock();
	double runtime = double(end - begin) / CLOCKS_PER_SEC / test_images.size();
	float accuracy = (float)right / (float)test_images.size();
	saveVector(vec, get_date_string());
	//std::cout << "similarity in total:" << simi << std::endl;
	std::cout << "dataset size is: " << test_images.size() << "  label size is: " << test_images.size() << std::endl;
	std::cout << "right prediction: " << right << " accuracy: " << accuracy << std::endl;
	std::cout << "runtime: " << runtime << std::endl;
	
}

void test_cnn_CV(network<sequential> &net, std::string folderName, int idx, const std::string name) {
	std::vector<label_t> test_labels;
	std::vector<vec_t> test_images;
	label_t predicted_label;

	std::cout << "reading images..." << std::endl;
	parse_mnist_images("../dataset/" + folderName + "/test-images-idx3-ubyte" + std::to_string(idx), &test_images, -1.0, 1.0, 0, 0);
	std::cout << "reading labels..." << std::endl;	parse_mnist_labels("../dataset/" + folderName + "/test-labels-idx1-ubyte" + std::to_string(idx), &test_labels);

	int right = 0;
	std::vector<std::vector<int>> vec(7, std::vector<int>(7, 0));

	//use cnn model
	clock_t begin = clock();
	for (size_t i = 0; i < test_images.size(); i++) {
		predicted_label = net.predict_label(test_images[i]);
		if (predicted_label == test_labels[i]) right++;
		vec[test_labels[i]][predicted_label]++;
		//std::cout << "similarity:" << net.predict_max_value(test_images[i]) << std::endl;
	}
	clock_t end = clock();
	double runtime = double(end - begin) / CLOCKS_PER_SEC / test_images.size();
	float accuracy = (float)right / (float)test_images.size();
	saveVector(vec, name);
	std::ofstream myfile("../experiment-data/result_" + name + ".txt");
	myfile << get_date_string() << "\n";
	myfile << "dataset size is: " << test_images.size() << "  label size is: " << test_images.size() << "\n";
	myfile << "right prediction: " << right << " accuracy: " << accuracy << "\n";
	myfile << "runtime: " << runtime << "\n";
	myfile.close();
}