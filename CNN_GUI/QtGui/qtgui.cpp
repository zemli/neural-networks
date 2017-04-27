#include "qtgui.h"


QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
    ui.setupUi(this);
	ui.startButton->setEnabled(false);
}

void QtGui::on_loadButton_clicked() {
	net.load("../Resources/cnn-model");
	ui.startButton->setEnabled(true);
	QMessageBox msgBox;
	msgBox.setText("The CNN model has been loaded.");
	msgBox.exec();
}

void QtGui::on_startButton_clicked() {	
	cv::CascadeClassifier face_cascade;
	if (!face_cascade.load("../Resources/haarcascade_frontalface_alt.xml")) {
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
	cv::Mat image, img;
	while (!quitLoop){
		cap >> image;
		if (!image.data) continue; // no data from video stream
		displayImage(image);
		cv::cvtColor(image, img, CV_RGB2GRAY);

		if (detectAndCrop(img, face_cascade))
			continue;

		//show cropped image
		cv::imshow("input to CNN", img);
		tiny_dnn::vec_t data;
		if (convert_image(img, data)) {
			QMessageBox msgBox;
			msgBox.setText("Failed to convert image.");
			msgBox.exec();
			continue;
		}

		//tiny_dnn::label_t predicted_label = net.predict_label(data);;
		emotions = net.predict(data);
		QStringList slist;
		//chart->setValues(emotions);
		for (int i = 0; i < 7; i++) {
			slist << QString::fromStdString(emotion_labels[i]) << ": " << QString::number(emotions[i]);
			if (i == 3) slist << "\n";
		}
		
		QString str = slist.join(" ");
		ui.resultLabel->setText(str);
		cv::waitKey(1);
	}

}

void QtGui::on_displayButton_clicked() {
	chart->display();
	ui.displayButton->setEnabled(false);
}

void QtGui::on_endButton_clicked() {
	quitLoop = true;
}

void QtGui::displayImage(cv::Mat image) {
	QImage qimg;
	//show img in mainwindow
	cv::cvtColor(image, image, CV_BGR2RGB);
	qimg = QImage((const unsigned char*)(image.data),
		image.cols, image.rows, QImage::Format_RGB888);
	// display on label
	ui.imageLabel->setPixmap(QPixmap::fromImage(qimg));
	// resize the label to fit the image
	ui.imageLabel->resize(ui.imageLabel->pixmap()->size());
}

