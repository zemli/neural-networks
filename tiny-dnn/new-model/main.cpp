#include <iostream>
#include "header.h"
#include<string>
using namespace std;

int main(){
	
	//network<sequential> net = construct_small_cnn();
	network<sequential> net;
	std::cout << "load models..." << std::endl;
	net.load("trained-model-simplified-1epoch");
	//net.load("trained-model-simplified-1epoch");

	//train_cnn(net);

	test_dnn(net);
	
	system("PAUSE");
	return 0;
}