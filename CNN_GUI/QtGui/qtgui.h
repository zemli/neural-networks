#pragma once

#ifndef QTGUI_H
#define QTGUI_H

#include <QtWidgets/QMainWindow>
#include "ui_qtgui.h"
#include <QMessageBox>

#include "process_data.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "tiny_dnn/tiny_dnn.h"

class QtGui : public QMainWindow
{
    Q_OBJECT

public:
    QtGui(QWidget *parent = Q_NULLPTR);

private slots:
    void on_startButton_clicked();
	void on_loadButton_clicked();

private:
    Ui::QtGuiClass ui;
	tiny_dnn::network<tiny_dnn::sequential> net;
    cv::Mat image; // the image variable
};

#endif //QTGUI_H
