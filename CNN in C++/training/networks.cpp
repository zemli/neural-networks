#include "header.h"

void printInfo(network<sequential> net) {
	for (size_t i = 0; i < net.depth(); i++) {
		std::cout << "#layer:" << i << "\n";
		std::cout << "layer type:" << net[i]->layer_type() << "\n";
		std::cout << "input:" << net[i]->in_data_size() << "(" << net[i]->in_data_shape() << ")\n";
		std::cout << "output:" << net[i]->out_data_size() << "(" << net[i]->out_data_shape() << ")\n";
	}
}

network<sequential> construct_deep_cnn() {
	network<sequential> net;
	
	return net;
}

network<sequential> construct_cnn() {
	network<sequential> net;
	//net << conv<relu>(48, 48, 3, 1, 32, padding::same)
	//	<< max_pool<tan_h>(48, 48, 32, 2)
	//	<< conv<relu>(24, 24, 3, 32, 64, padding::same)
	//	<< max_pool<tan_h>(24, 24, 64, 2)
	//	<< conv<relu>(12, 12, 3, 64, 128, padding::same)
	//	<< max_pool<tan_h>(12, 12, 128, 2)
	//	<< fc<relu>(6 * 6 * 128, 256)
	//	<< fc<relu>(256, 128)
	//	<< fc<softmax>(128, 7);

	net << conv<relu>(48, 48, 3, 1, 32, padding::same)
		<< max_pool<tan_h>(48, 48, 32, 2)
		<< conv<relu>(24, 24, 3, 32, 64, padding::same)
		<< max_pool<tan_h>(24, 24, 64, 2)
		<< conv<relu>(12, 12, 3, 64, 128, padding::same)
		<< max_pool<tan_h>(12, 12, 128, 2)
		<< conv<relu>(6, 6, 3, 128, 256, padding::same)
		<< max_pool<tan_h>(6, 6, 256, 2)
		<< fc<relu>(3 * 3 * 256, 256)
		<< fc<relu>(256, 128)
		<< fc<softmax>(128, 7);

	for (size_t i = 0; i < net.depth(); i++) {
		std::cout << "#layer:" << i << "\n";
		std::cout << "layer type:" << net[i]->layer_type() << "\n";
		std::cout << "input:" << net[i]->in_data_size() << "(" << net[i]->in_data_shape() << ")\n";
		std::cout << "output:" << net[i]->out_data_size() << "(" << net[i]->out_data_shape() << ")\n";
	}
	net.save("4conv-network");
	return net;
}

int train_cnn_ubyte(network<sequential> &net, std::vector<vec_t> train_images, std::vector<label_t> train_labels, float learning_rate, int batch_size, int epochs){

	adagrad opt;
	opt.alpha = learning_rate;

	std::cout << "training starting..." << std::endl;
	std::cout << "training set has " << train_images.size() << " images and " << train_labels.size() << " labels" << std::endl;
	progress_display disp(train_images.size());
	net.train<cross_entropy_multiclass>(opt, train_images, train_labels, batch_size, epochs);
	std::cout << "training finished" << std::endl;

	return 0;
}

int train_cnn_CV(std::string modelName, std::string folderName, int numOfFold, float learning_rate, int batch_size, int epochs) {	std::vector<label_t> alltrain_labels;
	std::vector<vec_t> alltrain_images;

	parse_mnist_images("../dataset/" + folderName + "/train-images-idx3-ubyte", &alltrain_images, -1.0, 1.0, 0, 0);
	std::cout << "reading images...  there are " << std::to_string(alltrain_images.size()) << " images" << std::endl;	parse_mnist_labels("../dataset/" + folderName + "/train-labels-idx1-ubyte", &alltrain_labels);	std::cout << "reading labels...  there are " << std::to_string(alltrain_labels.size()) << " labels" << std::endl;	for (int idx = 1; idx <= 2 /*changed from numOfFold*/; idx++) {		network<sequential> net;
		std::cout << "loading network: " << modelName << std::endl;
		net.load("../model/" + modelName);		std::vector<vec_t> train_images;		std::vector<label_t> train_labels;		std::vector<vec_t> test_images; 		std::vector<label_t> test_labels;		std::string name = "_" + modelName			+ "_" + folderName			+ "_" + std::to_string(learning_rate)			+ "_" + std::to_string(batch_size)			+ "_" + std::to_string(epochs)			+ "_" + std::to_string(idx);		//cross_validation("../dataset/" + folderName, 
		//	train_images, train_labels, 
		//	validation_images, validation_labels);

		if (parse_ubyte_cv(alltrain_images, alltrain_labels, train_images, train_labels, test_images, test_labels, idx, numOfFold)) return -1;
		
		if (train_cnn_ubyte(net, train_images, train_labels, learning_rate, batch_size, epochs)) {
			std::cout << "parse ubyte failed" << std::endl;
			return -1;
		}
		net.save("trained-model" + name);

		test_cnn_CV(net, test_images, test_labels, name);
	}	return 0;
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
	saveVector(vec, folderName + "_" + get_date_string());
	//std::cout << "similarity in total:" << simi << std::endl;
	std::cout << "dataset size is: " << test_images.size() << "  label size is: " << test_images.size() << std::endl;
	std::cout << "right prediction: " << right << " accuracy: " << accuracy << std::endl;
	std::cout << "runtime: " << runtime << std::endl;
	
}

void test_cnn_CV(network<sequential> &net, std::vector<vec_t> test_images, std::vector<label_t> test_labels, const std::string name) {

	label_t predicted_label;
	int right = 0;
	std::vector<std::vector<int>> vec(7, std::vector<int>(7, 0));
	//create a file stream
	std::ofstream myfile("../experiment-data/result_" + name + ".txt");
	myfile << get_date_string() << " ---- " << name << "\n";
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
	
	myfile << "dataset size is: " << test_images.size() << "  label size is: " << test_images.size() << "\n";
	myfile << "right prediction: " << right << " accuracy: " << accuracy << "\n";
	myfile << "runtime: " << runtime << "\n";
	myfile.close();
}