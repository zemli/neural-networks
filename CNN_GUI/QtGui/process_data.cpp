#include "process_data.h"

// convert image to vec_t
int convert_image(cv::Mat img, tiny_dnn::vec_t& data) {

	if (img.data == nullptr) return -1; // cannot open, or it's not an image

	cv::Mat_<uint8_t> resized = img;
	//predefined scale_max = 1, scale_min = -1
	std::transform(resized.begin(), resized.end(), std::back_inserter(data), [=](uint8_t c) { return c / 255.0 * 2 - 1; });

	return 0;
}

int detectAndCrop(cv::Mat &frame, cv::CascadeClassifier &face_cascade) {
	const int face_scale = 150;
	const int img_size = 48;

	std::vector<cv::Rect> faces;
	//detect face
	face_cascade.detectMultiScale(frame, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(face_scale, face_scale));
	if (faces.size() < 1)
		return -1;
	//crop image
	cv::Rect roi(faces[0].x, faces[0].y, faces[0].width, faces[0].width);
	cv::Mat image_roi = frame(roi);
	//resize
	resize(image_roi, frame, cv::Size(img_size, img_size));
	return 0;
}