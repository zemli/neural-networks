#include "BarChart.h"



BarChart::BarChart()
{
	set0 = new QtCharts::QBarSet("neutral");
	set1 = new QtCharts::QBarSet("anger");
	set2 = new QtCharts::QBarSet("disgust");
	set3 = new QtCharts::QBarSet("fear");
	set4 = new QtCharts::QBarSet("happy");
	set5 = new QtCharts::QBarSet("sadness");
	set6 = new QtCharts::QBarSet("surprise");

	series = new QtCharts::QBarSeries();
	
	categories << "neutral" << "anger" << "disgust" << "fear" << "happy" << "sadness" << "surprise";
	axis = new QtCharts::QBarCategoryAxis();
	axis->append(categories);
	chartView = new QtCharts::QChartView(chart);


}


BarChart::~BarChart()
{
	delete this;
}

void BarChart::setValues(tiny_dnn::vec_t result) {
	//set values from result vector
	//*set0 << 0.1;
	//*set1 << 0.055;
	//*set2 << 0.33;
	//*set3 << 0.5;
	//*set4 << 0.09;
	//*set5 << 0.01;
	//*set6 << 0.02;
}

void BarChart::display() {

	series->clear();
	series->append(set0);
	series->append(set1);
	series->append(set2);
	series->append(set3);
	series->append(set4);
	series->append(set5);
	series->append(set6);
	
	chart = new QtCharts::QChart();
	chart->addSeries(series);
	chart->setTitle("Simple horizontal barchart example");
	chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

	
	chart->createDefaultAxes();
	chart->setAxisX(axis, series);

	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);

	chartView->setRenderHint(QPainter::Antialiasing);

	QMainWindow window;
	window.setCentralWidget(chartView);
	window.resize(420, 300);
	window.show();
}
