#pragma once

#ifndef QTGUI_H
#define QTGUI_H

#include <QtWidgets/QMainWindow>
#include "ui_qtgui.h"
#include "BarChart.h"
#include <QMessageBox>

#include "process_data.h"

class BarChart;

class QtGui : public QMainWindow
{
    Q_OBJECT

public:
    QtGui(QWidget *parent = Q_NULLPTR);

private slots:
    void on_startButton_clicked();
	void on_loadButton_clicked();
	void on_displayButton_clicked();
	void on_endButton_clicked();

private:
    Ui::QtGuiClass ui;
	tiny_dnn::network<tiny_dnn::sequential> net; 
	bool quitLoop = false;
	tiny_dnn::vec_t emotions;
	BarChart *chart = new BarChart();

	void displayImage(cv::Mat image);
};

const std::vector<std::string> emotion_labels{ "neutral", "anger", "disgust", "fear", "happy", "sadness", "surprise" };

#endif //QTGUI_H
