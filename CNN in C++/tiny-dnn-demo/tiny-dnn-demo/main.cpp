#include <iostream>
#include "header.h"

int main(){
	network<sequential> net = construct_cnn();
	
	train_cnn(net);
	//use cnn model
	vec_t in = { 1, 0.5 };
	std::cout << "result:" << net.predict_label(in) << std::endl;
	std::cout << "similarity:" << net.predict_max_value(in) << std::endl;	
	system("PAUSE");
	return 0;
}