#include "header.h"


int demo(network<sequential> &net) {
	const std::string imagefilename = "C:/Users/Zeming/Desktop/face1.jpg";
	cv::Mat img = cv::imread(imagefilename, cv::IMREAD_GRAYSCALE);
	if (!img.data) {
		std::cout << "open image failed" << std::endl;
		return -1;
	}

	const std::string xmlPath = "../sources/";
	const std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
	cv::CascadeClassifier face_cascade;
	if (!face_cascade.load(xmlPath + face_cascade_name)) {
		std::cout << "(!)Error loading Cascade Classifier" << std::endl;
		return -1;
	}

	if (detectAndCrop(img, face_cascade)) {
		std::cout << "preprocessing failed" << std::endl;
		return -1;
	}

	vec_t data;
	if (convert_image(img, data)) {
		std::cout << "converting data failed" << std::endl;
		return -1;
	}

	label_t predicted_label = net.predict_label(data);
	std::cout << "label is " << emotion_labels[predicted_label] << std::endl;
	return 0;
}