#include "qtgui.h"


QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
    ui.setupUi(this);
	ui.startButton->setEnabled(false);
}

void QtGui::on_loadButton_clicked() {
	net.load("./Resources/trained-new5-model");
	ui.startButton->setEnabled(true);
	QMessageBox msgBox;
	msgBox.setText("The CNN model has been loaded.");
	msgBox.exec();
}

void QtGui::on_startButton_clicked() {	
	cv::CascadeClassifier face_cascade;
	if (!face_cascade.load("./Resources/haarcascade_frontalface_alt.xml")) {
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
	quitLoop = false;
	while (!quitLoop){
		cap >> image;
		if (!image.data) continue; // no data from video stream

		cv::cvtColor(image, img, CV_RGB2GRAY);
		//show img in mainwindow
		cv::cvtColor(image, image, CV_BGR2RGB);
		qimg = QImage((const unsigned char*)(image.data),
			image.cols, image.rows, QImage::Format_RGB888);
		// display on label
		ui.imageLabel->setPixmap(QPixmap::fromImage(qimg));
		// resize the label to fit the image
		ui.imageLabel->resize(ui.imageLabel->pixmap()->size());

		if (detectAndCrop(img, face_cascade)) {
			QMessageBox msgBox;
			msgBox.setText("Failed to detect and crop.");
			msgBox.exec();
			continue;
		}
		//show cropped image
		cv::imshow(" ", img);
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

		//get 7 scores and predicted label
		//for (int i = 0; i < 7; i++) {
		//	scores.emplace_back((res[i]), i);
		//	if (res[i] > max_prob) {
		//		max_prob = res[i];
		//		predicted_label = i;
		//	}
		//}
		cv::waitKey(1);
	}

}

void QtGui::on_endButton_clicked() {
	quitLoop = true;
}
