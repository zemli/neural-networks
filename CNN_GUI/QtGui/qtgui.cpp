#include "qtgui.h"


QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
    ui.setupUi(this);
	ui.startButton->setEnabled(false);
}

void QtGui::on_loadButton_clicked()
{
	net.load("../sources/trained-new-model");
	ui.startButton->setEnabled(true);
	QMessageBox msgBox;
	msgBox.setText("The CNN model has been loaded.");
	msgBox.exec();
}

void QtGui::on_startButton_clicked()
{	
	const std::string xmlPath = "../sources/";
	const std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
	cv::CascadeClassifier face_cascade;
	if (!face_cascade.load(xmlPath + face_cascade_name)) {
		QMessageBox msgBox;
		msgBox.setText("Cascade Classifier failed to be loaded.");
		msgBox.exec();
		return;
	}

	cv::VideoCapture cap;
	if (!cap.open(0)) {
		QMessageBox msgBox;
		msgBox.setText("Webcam failed to be opened.");
		msgBox.exec();
		return;
	}
	while (true){
		cap >> image;
		if (!image.data) continue; // end of video stream
		cv::imshow("original video", image);
		cv::Mat img;
		cv::cvtColor(image, img, CV_RGB2GRAY);

		if (detectAndCrop(img, face_cascade)) {
			QMessageBox msgBox;
			msgBox.setText("Failed to detect and crop.");
			msgBox.exec();
			continue;
		}

		tiny_dnn::vec_t data;
		if (convert_image(img, data)) {
			QMessageBox msgBox;
			msgBox.setText("Failed to convert image.");
			msgBox.exec();
			continue;
		}

		float max_prob = 0.0;
		tiny_dnn::label_t predicted_label;

		std::vector<std::pair<double, int>> scores;
		tiny_dnn::vec_t res = net.predict(data);
		//if (res.empty()) {
		//	QMessageBox msgBox;
		//	msgBox.setText("Failed to recognize emotions.");
		//	msgBox.exec();
		//	continue;
		//}
		for (int i = 0; i < 7; i++) {
			scores.emplace_back((res[i]), i);
			if (res[i] > max_prob) {
				max_prob = res[i];
				predicted_label = i;
			}
		}


		if (cv::waitKey(10) >= 0) break; // stop capturing by pressing any key
	}

}
