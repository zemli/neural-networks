#include <iostream>
#include "header.h"
#include<string>
using namespace std;

int main(){
	network<sequential> net;
	//network<sequential> net = construct_cnn();
	net.load("myNetwork");

	train_cnn(net);

	test_dnn(net);
	
	system("PAUSE");
	return 0;
}