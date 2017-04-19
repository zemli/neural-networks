#include "header.h"

int main(int argc, char **argv) {
	std::string folderName;
	std::string modelName;

	if (argc < 2) {
		std::cerr << "Usage : " << argv[0] 
			<< "--option model_name folder_name (e.g. --test trained-new-model small-classified)" 
			<< std::endl;
		system("PAUSE");
		return -1;
	}

	for (int count = 1; count+1 < argc; count++) {
		std::string argname(argv[count]);
		if (argname == "--construct") {
			std::cout << argv[count] << " mode selected" << std::endl;
			network<sequential> net = construct_small_cnn();
			count++;
		}
		else if (argname == "--train") {
			std::cout << argv[count] << " mode selected" << std::endl;
			if (count + 2 < argc) {
				modelName = argv[++count];
				network<sequential> net;
				std::cout << "loading model: "<< modelName << std::endl;
				net.load("../model/" + modelName);

				folderName = argv[++count];
				std::cout << "training model on " << folderName << " dataset" << std::endl;
				train_cnn(net, folderName);
			}
			else {
				std::cerr << "Usage : --option " << "model_name (example:smallNetwork)" << " folder_name (example:classified)"
					<< std::endl;
				system("PAUSE");
				return -1;
			}
		}
		else if (argname == "--test") {
			std::cout << argv[count] << " mode selected" << std::endl;
			if (count + 2 < argc) {
				modelName = argv[++count];
				network<sequential> net;
				std::cout << "loading model: " << modelName << std::endl;
				net.load("../model/" + modelName);
				showInfo(net);
				folderName = argv[++count];
				std::cout << "testing model on " << folderName << " dataset" << std::endl;
				test_cnn(net, folderName);
			}
			else {
				std::cerr << "Usage : --option " << "model_name (example:smallNetwork)" << " folder_name (example:classified)"<< std::endl;
				system("PAUSE");
				return -1;
			}
		}
		else if (argname == "--demo") {
			std::cout << argv[count] << " mode selected" << std::endl;
			if (count + 2 < argc) {
				modelName = argv[++count];
				network<sequential> net;
				//comment for speeding up test
				std::cout << "loading model: " << modelName << std::endl;
				net.load("../model/" + modelName);
				showInfo(net);

				if (demo(net)) {
					std::cout << "demo failed" << std::endl;
					system("PAUSE");
					return -1;
				}
				count++;	 //for skipping one parameter. making the format same
				/*folderName = argv[++count];
				std::cout << "testing model on " << folderName << " dataset" << std::endl;
				test_cnn(net, folderName);*/
			}
			else {
				std::cerr << "Usage : --option " << "model_name (example:smallNetwork)" << " folder_name (example:classified)" << std::endl;
				system("PAUSE");
				return -1;
			}
		}
		else {
			std::cerr << "Usage : " << argv[0] << "--option model_name folder_name"<< std::endl;
			system("PAUSE");
			return -1;
		}
	}
	
	system("PAUSE");
	return 0;
}