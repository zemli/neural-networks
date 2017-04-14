#include "header.h"

network<sequential> construct_cnn() {
	network<sequential> net;
	net << layers::fc<tan_h>(2, 3) << layers::fc<softmax>(3, 4);

	for (size_t i = 0; i < net.depth(); i++) {
		cout << "#layer:" << i << "\n";
		cout << "layer type:" << net[i]->layer_type() << "\n";
		cout << "input:" << net[i]->in_data_size() << "(" << net[i]->in_data_shape() << ")\n";
		cout << "output:" << net[i]->out_data_size() << "(" << net[i]->out_data_shape() <<")\n";
	}
	
	return net;
}

void train_cnn(network<sequential> &net){
	// input_data[0] should be classified to id:3
	// input_data[1] should be classified to id:1
	adagrad opt;
	std::vector<vec_t> input_data{ { 1, 0 }, { 0, 2 } };
	std::vector<label_t> desired_out{ 3, 1 };
	size_t batch_size = 1;
	size_t epochs = 30;
	net.train<mse>(opt, input_data, desired_out, batch_size, epochs);

}
