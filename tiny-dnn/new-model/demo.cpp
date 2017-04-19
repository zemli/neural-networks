#include "header.h"


int demo(network<sequential> &net) {
	const std::string imagefilename = "C:/Users/Zeming/Desktop/face.jpg";
	cv::Mat img = cv::imread(imagefilename, cv::IMREAD_GRAYSCALE);
	cv::imshow("non-processed image", img);
	if (detectAndCrop(img)) {
		std::cout << "preprocessing failed" << std::endl;
		return -1;
	}
	cv::imshow("processed image", img);
	vec_t data;
	if (convert_image(img, data)) {
		std::cout << "converting data failed" << std::endl;
		return -1;
	}

	label_t predicted_label = net.predict_label(data);
	std::cout << "label is " << predicted_label << std::endl;
	return 0;
}