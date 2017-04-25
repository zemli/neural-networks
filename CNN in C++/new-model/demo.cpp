#include "header.h"

int demo(network<sequential> &net, int camIndex) {
	cv::VideoCapture cap(camIndex);
	if (!cap.isOpened())
	{
		std::cout << "camera is not opened";
		return -1;
	}
	cv::Mat img;
	const std::string xmlPath = "../sources/";
	const std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
	cv::CascadeClassifier face_cascade;
	if (!face_cascade.load(xmlPath + face_cascade_name)) {
		std::cout << "(!)Error loading Cascade Classifier" << std::endl;
		return -1;
	}

	while (true) {
		cap >> img;

		if (!img.data) {
			std::cout << "no data from webcam" << std::endl;
			continue;
		}
		cv::cvtColor(img, img, CV_RGB2GRAY);

		if (detectAndCrop(img, face_cascade)) {
			std::cout << "face detection failed" << std::endl;
			continue;
		}

		vec_t data;
		if (convert_image(img, data)) {
			std::cout << "converting data failed" << std::endl;
			continue;
		}

		/*label_t predicted_label = net.predict_label(data);
		std::cout << "label is " << emotion_labels[predicted_label] << std::endl;*/
		float max_prob = 0.0;
		label_t predicted_label;

		std::vector<std::pair<double, int>> scores;
		vec_t res = net.predict(data);
		for (int i = 0; i < 7; i++) {
			scores.emplace_back((res[i]), i);
			if (res[i] > max_prob) {
				max_prob = res[i];
				predicted_label = i;
			}
		}

		for (int i = 0; i < 7; i++)
			std::cout << emotion_labels[scores[i].second] << ", " << scores[i].first << "   ";
		std::cout << "emotion is " << emotion_labels[predicted_label] << std::endl;

		if (cv::waitKey(1) >= 0) break;
	}

	
	
	return 0;
}