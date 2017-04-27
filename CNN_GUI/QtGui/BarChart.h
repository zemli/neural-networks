#pragma once
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>

#include "tiny_dnn/tiny_dnn.h"

class BarChart
{
public:
	BarChart();
	~BarChart();
	void display();
	void setValues(tiny_dnn::vec_t result);

private:
	tiny_dnn::vec_t result;
	QtCharts::QBarSet *set0;
	QtCharts::QBarSet *set1;
	QtCharts::QBarSet *set2;
	QtCharts::QBarSet *set3;
	QtCharts::QBarSet *set4;
	QtCharts::QBarSet *set5;
	QtCharts::QBarSet *set6;

	QtCharts::QBarSeries *series;
	QtCharts::QChart *chart;
	QStringList categories;
	QtCharts::QBarCategoryAxis *axis;
	QtCharts::QChartView *chartView;
};

