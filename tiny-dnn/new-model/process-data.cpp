#include "header.h"


void saveVector(std::vector<std::vector<int>> &vec) {
	std::ofstream myfile("../experiment-data/confusion-matrix.txt");
	if (myfile.is_open()){
		myfile << "confusion-matrix\n";
		for (int row = 0; row < 7; row++) {
			for(int col = 0; col < 7; col++)
				myfile << vec[row][col] << " ";
			myfile << '\n';
		}
		myfile.close();
		std::cout << "confusion matrix written to file \n";
	}
	else std::cout << "Unable to open file \n";

}



