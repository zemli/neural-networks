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

// convert image to vec_t
int convert_image(cv::Mat img, vec_t& data) {
	
	if (img.data == nullptr) return -1; // cannot open, or it's not an image

	cv::Mat_<uint8_t> resized = img;
	//cv::resize(img, resized, cv::Size(w, h));

	std::transform(resized.begin(), resized.end(), std::back_inserter(data), [=](uint8_t c) { return c/255.0*2-1; });
																		//predefined scale_max = 1, scale_min = -1
	//data.push_back(d);

	return 0;
}

int detectAndCrop(cv::Mat &frame, cv::CascadeClassifier &face_cascade) {

	std::vector<cv::Rect> faces;

	//detect face
	face_cascade.detectMultiScale(frame, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(150, 150));
	//crop image
	cv::Rect roi(faces[0].x, faces[0].y, faces[0].width, faces[0].width);
	cv::Mat image_roi = frame(roi);
	//resize
	cv::Size size(48, 48);
	resize(image_roi, frame, size);
	return 0;
}


