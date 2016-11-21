#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "ui_mainwindow.h"
#include "copenni.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
using namespace cv;
using namespace std;

#define DEPTH_SCALE_FACTOR 255./4096.
#define ROI_HAND_WIDTH 140
#define ROI_HAND_HEIGHT 140
#define MEDIAN_BLUR_K 5
#define XRES  640
#define YRES  480
#define DEPTH_SEGMENT_THRESH 5
#define MAX_HANDS_COLOR 10
#define MAX_HANDS_NUMBER  10

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

	COpenNI openni;

private:
	Ui::MainWindowClass ui;

	QGraphicsScene *depthScene;
	QImage qDepthImage;
	QImage qScaledDepthImage;
	QGraphicsPixmapItem *depthItem;

	QGraphicsScene *colorScene;
	QImage qColorImage;
	QImage qScaledColorImage;
	QGraphicsPixmapItem *colorItem;

	QGraphicsScene *handScene;
	QImage qHandImage;
	QImage qScaledHandImage;
	QGraphicsPixmapItem *handItem;

	QGraphicsScene *matchScene;
	QImage qMatchImage;
	QImage qScaledMatchImage;
	QGraphicsPixmapItem *matchItem;

	unsigned int convex_number_above_point1;
	unsigned int concave_number_above_point1;
	unsigned int convex_number_above_point2;
	unsigned int concave_number_above_point2;
	unsigned int convex_assist_above_point1;
	unsigned int convex_assist_above_point2;
	unsigned int point_y1;
	unsigned int point_y2;
	int number_result;
	bool recognition_flag;  //开始手部数字识别的标志
	vector<Scalar> color_array;//采用默认的10种颜色

	vector<unsigned int> hand_depth;
	vector<Rect> hands_roi;



protected:
	void timerEvent(QTimerEvent *);

public slots:
	void on_actionExit_triggered();
	void on_actionStart_triggered();
	void on_actionStop_triggered();
	void on_actionAboutQt_triggered();
	void on_actionAboutSDK_triggered();
};

#endif // MAINWINDOW_H
