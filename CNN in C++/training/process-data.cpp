#include "header.h"

void saveVector(std::vector<std::vector<int>> &vec, const std::string name) {
	std::ofstream myfile("../experiment-data/confusion-matrix_"+ name + ".txt");
	if (myfile.is_open()){
		myfile << "confusion-matrix\n";
		myfile << get_date_string() << std::endl;
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

//get last label_t index in each class
int getLabelIdx(std::vector<label_t> train_labels, std::vector<size_t> &classIdx) {
	label_t val = 1;
	for (label_t it:train_labels) {
		if (it == val) {
			train_labels.push_back((size_t)it - 1);
			val++;
		}
	}
	classIdx.push_back((size_t)train_labels.size()-1);
	return 0;
}

int parse_ubyte_cv(std::vector<vec_t> train_images, std::vector<label_t> train_labels, 
	std::vector<vec_t> &sub_train_images, std::vector<label_t> &sub_train_labels, 
	std::vector<size_t> classIdx, int numOfFold) {
	int amount = classIdx[0] + 1;
	//TODO

	return 0;
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

// convert all images found in directory to vec_t
//void convert_images(const std::string & directory, std::vector<vec_t>& data) {
//	std::map<int, vec_t> example;
//	std::map<int, std::vector<vec_t> > myMap;
//
//
//
//	boost::filesystem::path dpath(directory);
//	int index = 0;
//	BOOST_FOREACH(const boost::filesystem::path& p,
//		std::make_pair(boost::filesystem::directory_iterator(dpath), boost::filesystem::directory_iterator())) {
//		if (boost::filesystem::is_directory(p)) continue;
//		auto img = cv::imread(p.string(), cv::IMREAD_GRAYSCALE);
//		convert_image(img, data[index++]);
//	}
//}

void cross_validation(const std::string directory, std::vector<vec_t> &train_images, std::vector<label_t> &train_labels, std::vector<vec_t> &validation_images, std::vector<label_t> &validation_labels) {
	const size_t numOfEmotions = 7;
	std::string time_str = get_date_string();
	std::ofstream training_file(directory + "/" + "profiles" + "/" + "training" + time_str + ".txt");
	std::ofstream validation_file(directory + "/" + "profiles" + "/" + "validation" + time_str + ".txt");

	for (int emotion_idx = 0; emotion_idx < numOfEmotions; emotion_idx++) {
		cv::String path = directory + "/" + std::to_string(emotion_idx) + "/*.png";
		std::vector<cv::String> filenames;
		cv::glob(path, filenames);
		srand((unsigned int)time(NULL));
		for (size_t i = 0; i < filenames.size(); ++i) {
			cv::Mat img = cv::imread(filenames[i]);
			vec_t data;
			label_t label = static_cast<uint32_t>(emotion_idx);
			if (img.empty()) {
				std::cout << "failed to open img" << std::endl;
				continue;
			}
			if (convert_image(img, data)) { //zero-normolize data
				std::cout << "converting data failed" << std::endl;
				continue;
			}

			int random = rand() % 100 + 1; //random number from 1 to 100
			if (random <= 75) {	// 0.75 = 0.6 / (0.2+0.6)
				train_images.push_back(data);
				train_labels.push_back(label);
				training_file << filenames[i] << " label is " << std::to_string(emotion_idx) <<"\n";
			}
			else {
				validation_images.push_back(data);
				validation_labels.push_back(label);
				validation_file << filenames[i] << " label is " << std::to_string(emotion_idx) << "\n";
			}
		}
		
	}
	training_file.close();
	validation_file.close();
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





