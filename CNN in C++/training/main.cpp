#include "header.h"

int main_exe(int argc, char **argv) {
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
			network<sequential> net = construct_cnn();
			count++;
		}
		else if (argname == "--train_CV") {
			int numOfFold, batch_size, epochs;
			float learning_rate;
			std::cout << argv[count] << " mode selected" << std::endl;
			if (count + 6 < argc) {
				modelName = argv[++count];
				folderName = argv[++count];
				std::cout << "training model on " << folderName << " dataset" << std::endl;

				numOfFold = atoi(argv[++count]);
				
				learning_rate = (float)atof(argv[++count]);
				batch_size = atoi(argv[++count]);
				epochs = atoi(argv[++count]);

				train_cnn_CV(modelName, folderName, numOfFold,  learning_rate, batch_size, epochs);
			}
		}
		else if (argname == "--train_final") {
			int batch_size, epochs;
			float learning_rate;
			std::cout << argv[count] << " mode selected" << std::endl;
			if (count + 5 < argc) {
				modelName = argv[++count];
				folderName = argv[++count];
				std::cout << "training model on " << folderName << " dataset" << std::endl;
				network<sequential> net;
				std::cout << "loading network: " << modelName << std::endl;
				net.load("../model/" + modelName);

				learning_rate = (float)atof(argv[++count]);
				batch_size = atoi(argv[++count]);
				epochs = atoi(argv[++count]);

				train_cnn_ubyte(net, folderName, -1/*means not for cross-validation*/, learning_rate, batch_size, epochs);
				std::string name = "_" + folderName					+ "_" + std::to_string(learning_rate)					+ "_" + std::to_string(batch_size)					+ "_" + std::to_string(epochs)					+ "_" + "final";
				net.save("trained-model" + name);
			}
		}
		else if (argname == "--test") {
			std::cout << argv[count] << " mode selected" << std::endl;
			if (count + 2 < argc) {
				modelName = argv[++count];
				network<sequential> net;
				std::cout << "loading model: " << modelName << std::endl;
				net.load("../model/" + modelName);
				printInfo(net);
				folderName = argv[++count];
				std::cout << "testing model on " << folderName << " dataset" << std::endl;
				test_cnn(net, folderName);
			}
		}
		else if (argname == "--demo") {
			std::cout << argv[count] << " mode selected" << std::endl;
			if (count + 2 < argc) {
				modelName = argv[++count];
				int camIndex = std::stoi(argv[++count], nullptr, 10);
				network<sequential> net;
				//comment for speeding up test
				std::cout << "loading model: " << modelName << std::endl;
				net.load("../model/" + modelName);
				printInfo(net);

				while (true) {
					try {
						if (demo(net, camIndex)) {
							std::cout << "demo failed" << std::endl;
							system("PAUSE");
							//return -1;
						}
					}
					catch (const std::exception& e) {
						std::cout << e.what() << std::endl;
					}
					std::cout << "Do you want to repeat demo now? (y/n)" << std::endl;
					char choice;
					std::cin >> choice;
					if (choice != 'Y' && choice != 'y') {
						std::cout << "ending" << std::endl;
						system("PAUSE");
						return -1;
					}
				}
				
			}
		}
		else {
			std::cerr << "Usage : " << argv[0] << " --option model_name folder_name"<< std::endl;
			std::cerr << "e.g. : " << argv[0] << " --test cnn-trained-model ck_dataset  or --demo trained-new-model 0" << std::endl;
			std::cerr << "List of modes : " << " --construct, --train_CV, --train_final, --test, --demo" << std::endl;
			system("PAUSE");
			return -1;
		}
	}

	return 0;
}

int main(int argc, char **argv) {
	int code;
	try {
		code = main_exe(argc, argv);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		code = -1;
	}
	//system("PAUSE");
	return code;
}