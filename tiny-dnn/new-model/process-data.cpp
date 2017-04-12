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
void convert_image(std::string imagefilename,
	double scale,
	int w,
	int h,
	std::vector<vec_t>& data)
{
	auto img = cv::imread(imagefilename, cv::IMREAD_GRAYSCALE);
	if (img.data == nullptr) return; // cannot open, or it's not an image

	cv::Mat_<uint8_t> resized;
	cv::resize(img, resized, cv::Size(w, h));
	vec_t d;

	std::transform(resized.begin(), resized.end(), std::back_inserter(d),
		[=](uint8_t c) { return c * scale; });
	data.push_back(d);
}


