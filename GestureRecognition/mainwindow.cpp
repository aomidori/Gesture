#include "mainwindow.h"
#define DEPTH_SCALE_FACTOR 255./4096.
#define ROI_HAND_WIDTH 140
#define ROI_HAND_HEIGHT 140
#define MEDIAN_BLUR_K 5
#define XRES  640
#define YRES  480
#define DEPTH_SEGMENT_THRESH 5
#define MAX_HANDS_COLOR 10
#define MAX_HANDS_NUMBER  10
#define HAND_LIKELY_AREA 2000
#define DELTA_POINT_DISTENCE 25     //手部中心点1和中心点2距离的阈值
#define SEGMENT_POINT1_DISTANCE 27  //凸点与手部中心点1远近距离的阈值
#define SEGMENT_POINT2_DISTANCE 30  //凸点与手部中心点2远近距离的阈值

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	depthScene=new QGraphicsScene;
	depthItem=new QGraphicsPixmapItem(0,depthScene);
	ui.depthView->setScene(depthScene);

	colorScene=new QGraphicsScene;
	colorItem=new QGraphicsPixmapItem(0,colorScene);
	ui.colorView->setScene(colorScene);

	handScene=new QGraphicsScene;
	handItem=new QGraphicsPixmapItem(0,handScene);
	ui.handView->setScene(handScene);

	matchScene=new QGraphicsScene;
	matchItem=new QGraphicsPixmapItem(0,matchScene);
	ui.matchView->setScene(matchScene);

	convex_number_above_point1 = 0;
	concave_number_above_point1 = 0;
	convex_number_above_point2 = 0;
	concave_number_above_point2 = 0;
	convex_assist_above_point1 = 0;
	convex_assist_above_point2 = 0;
	point_y1 = 0;
	point_y2 = 0;
	number_result = -1;
	recognition_flag = false;  //开始手部数字识别的标志

	{
		color_array.push_back(Scalar(255, 0, 0));
		color_array.push_back(Scalar(0, 255, 0));
		color_array.push_back(Scalar(0, 0, 255));
		color_array.push_back(Scalar(255, 0, 255));
		color_array.push_back(Scalar(255, 255, 0));
		color_array.push_back(Scalar(0, 255, 255));
		color_array.push_back(Scalar(128, 255, 0));
		color_array.push_back(Scalar(0, 128, 255));
		color_array.push_back(Scalar(255, 0, 128));
		color_array.push_back(Scalar(255, 128, 255));
	}


	{
		hand_depth.push_back(0);
		hand_depth.push_back(0);
		hand_depth.push_back(0);
		hand_depth.push_back(0);
		hand_depth.push_back(0);
		hand_depth.push_back(0);
		hand_depth.push_back(0);
		hand_depth.push_back(0);
		hand_depth.push_back(0);
		hand_depth.push_back(0);
	}

	{
		hands_roi.push_back(Rect(XRES/2, YRES/2, ROI_HAND_WIDTH, ROI_HAND_HEIGHT));
		hands_roi.push_back(Rect(XRES/2, YRES/2, ROI_HAND_WIDTH, ROI_HAND_HEIGHT));
		hands_roi.push_back(Rect(XRES/2, YRES/2, ROI_HAND_WIDTH, ROI_HAND_HEIGHT));
		hands_roi.push_back(Rect(XRES/2, YRES/2, ROI_HAND_WIDTH, ROI_HAND_HEIGHT));
		hands_roi.push_back(Rect(XRES/2, YRES/2, ROI_HAND_WIDTH, ROI_HAND_HEIGHT));
		hands_roi.push_back(Rect(XRES/2, YRES/2, ROI_HAND_WIDTH, ROI_HAND_HEIGHT));
		hands_roi.push_back(Rect(XRES/2, YRES/2, ROI_HAND_WIDTH, ROI_HAND_HEIGHT));
		hands_roi.push_back(Rect(XRES/2, YRES/2, ROI_HAND_WIDTH, ROI_HAND_HEIGHT));
		hands_roi.push_back(Rect(XRES/2, YRES/2, ROI_HAND_WIDTH, ROI_HAND_HEIGHT));
		hands_roi.push_back(Rect(XRES/2, YRES/2, ROI_HAND_WIDTH, ROI_HAND_HEIGHT));
	}
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_actionExit_triggered()
{
	close();
}

void MainWindow::on_actionStart_triggered()
{
	if (!openni.Initial())
	{
		return;
	}

	if (!openni.Start())
	{
		return;
	}
	cout<<"Start Kinect..."<<endl;
	setStatusTip("Start Kinect...");
	startTimer(30);
}

void MainWindow::timerEvent(QTimerEvent *)
{
	if (!openni.UpdateData())
	{
		return;
	}

	Mat depth_image_src(openni.depth_metadata_.YRes(), openni.depth_metadata_.XRes(),CV_16UC1, (char *)openni.depth_metadata_.Data());//因为kinect获取到的深度图像实际上是无符号的16位数据
	Mat depth_image;
	depth_image_src.convertTo(depth_image, CV_8U, DEPTH_SCALE_FACTOR);
	
	qDepthImage=QImage((const unsigned char*)depth_image.data,depth_image.cols,depth_image.rows,depth_image.channels()*depth_image.cols,QImage::Format_Indexed8);
	qScaledDepthImage = qDepthImage.scaled(ui.depthView->width()-5,ui.depthView->height()-5,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	depthItem->setPixmap(QPixmap::fromImage(qScaledDepthImage));

	Mat color_image_src(openni.image_metadata_.YRes(), openni.image_metadata_.XRes(),CV_8UC3, (char *)openni.image_metadata_.Data());
	Mat color_image;
	cvtColor(color_image_src, color_image, CV_RGB2BGR);

	qColorImage=QImage((const unsigned char*)color_image_src.data,color_image_src.cols,color_image_src.rows,color_image_src.channels()*color_image_src.cols,QImage::Format_RGB888);
	qScaledColorImage = qColorImage.scaled(ui.colorView->width()-5,ui.colorView->height()-5,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	colorItem->setPixmap(QPixmap::fromImage(qScaledColorImage));

	Mat hand_segment_mask(color_image.size(), CV_8UC1, Scalar::all(0));

	for(auto itUser = openni.hand_points_.cbegin(); itUser != openni.hand_points_.cend(); ++itUser) 
	{

		point_y1 = itUser->second.Y;
		point_y2 = itUser->second.Y + DELTA_POINT_DISTENCE;
		circle(color_image, Point(itUser->second.X, itUser->second.Y),5, color_array.at(itUser->first % color_array.size()), 3, 8);

		/*设置不同手部的深度*/
		hand_depth.at(itUser->first % MAX_HANDS_COLOR) = (unsigned int)(itUser->second.Z* DEPTH_SCALE_FACTOR);//itUser->first会导致程序出现bug

		/*设置不同手部的不同感兴趣区域*/
		hands_roi.at(itUser->first % MAX_HANDS_NUMBER) = Rect(itUser->second.X - ROI_HAND_WIDTH/2, itUser->second.Y - ROI_HAND_HEIGHT/2,ROI_HAND_WIDTH, ROI_HAND_HEIGHT);
		hands_roi.at(itUser->first % MAX_HANDS_NUMBER).x =  itUser->second.X - ROI_HAND_WIDTH/2;
		hands_roi.at(itUser->first % MAX_HANDS_NUMBER).y =  itUser->second.Y - ROI_HAND_HEIGHT/2;
		hands_roi.at(itUser->first % MAX_HANDS_NUMBER).width = ROI_HAND_WIDTH;
		hands_roi.at(itUser->first % MAX_HANDS_NUMBER).height = ROI_HAND_HEIGHT;
		if(hands_roi.at(itUser->first % MAX_HANDS_NUMBER).x <= 0)
			hands_roi.at(itUser->first % MAX_HANDS_NUMBER).x  = 0;
		if(hands_roi.at(itUser->first % MAX_HANDS_NUMBER).x > XRES)
			hands_roi.at(itUser->first % MAX_HANDS_NUMBER).x =  XRES;
		if(hands_roi.at(itUser->first % MAX_HANDS_NUMBER).y <= 0)
			hands_roi.at(itUser->first % MAX_HANDS_NUMBER).y = 0;
		if(hands_roi.at(itUser->first % MAX_HANDS_NUMBER).y > YRES)
			hands_roi.at(itUser->first % MAX_HANDS_NUMBER).y =  YRES;
	}

	

	for(auto itUser = openni.hand_points_.cbegin(); itUser != openni.hand_points_.cend(); ++itUser) 
	{
		for(int i = hands_roi.at(itUser->first % MAX_HANDS_NUMBER).x; i < (std::min)(hands_roi.at(itUser->first % MAX_HANDS_NUMBER).x+hands_roi.at(itUser->first % MAX_HANDS_NUMBER).width, XRES); i++)
			for(int j = hands_roi.at(itUser->first % MAX_HANDS_NUMBER).y; j < (std::min)(hands_roi.at(itUser->first % MAX_HANDS_NUMBER).y+hands_roi.at(itUser->first % MAX_HANDS_NUMBER).height, YRES); j++)
			{
				hand_segment_mask.at<unsigned char>(j, i) = ((hand_depth.at(itUser->first % MAX_HANDS_NUMBER)-DEPTH_SEGMENT_THRESH) < depth_image.at<unsigned char>(j, i))
					& ((hand_depth.at(itUser->first % MAX_HANDS_NUMBER)+DEPTH_SEGMENT_THRESH) > depth_image.at<unsigned char>(j,i));
			}
	}
	medianBlur(hand_segment_mask, hand_segment_mask, MEDIAN_BLUR_K);
	Mat hand_segment(color_image.size(), CV_8UC3);
	color_image.copyTo(hand_segment, hand_segment_mask);

	/*对mask图像进行轮廓提取,并在手势识别图像中画出来*/
	std::vector< std::vector<Point> > contours;
	findContours(hand_segment_mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);//找出mask图像的轮廓
	Mat hand_recognition_image = Mat::zeros(color_image.rows, color_image.cols, CV_8UC3);

	for(int i = 0; i < contours.size(); i++) 
	{  //只有在检测到轮廓时才会去求它的多边形，凸包集，凹陷集
		recognition_flag = true;
		/*找出轮廓图像多边形拟合曲线*/
		Mat contour_mat = Mat(contours[i]);
		if(contourArea(contour_mat) > HAND_LIKELY_AREA) 
		{   //比较有可能像手的区域
			std::vector<Point> approx_poly_curve;
			approxPolyDP(contour_mat, approx_poly_curve, 10, true);//找出轮廓的多边形拟合曲线
			std::vector< std::vector<Point> > approx_poly_curve_debug;
			approx_poly_curve_debug.push_back(approx_poly_curve);

			drawContours(hand_recognition_image, contours, i, Scalar(255, 0, 0), 1, 8); //画出轮廓
			//            drawContours(hand_recognition_image, approx_poly_curve_debug, 0, Scalar(256, 128, 128), 1, 8); //画出多边形拟合曲线

			/*对求出的多边形拟合曲线求出其凸包集*/
			vector<int> hull;
			convexHull(Mat(approx_poly_curve), hull, true);
			for(int i = 0; i < hull.size(); i++)
			{
				circle(hand_recognition_image, approx_poly_curve[hull[i]], 2, Scalar(0, 255, 0), 2, 8);

				/*统计在中心点1以上凸点的个数*/
				if(approx_poly_curve[hull[i]].y <= point_y1)
				{
					/*统计凸点与中心点1的y轴距离*/
					long dis_point1 = abs(long(point_y1 - approx_poly_curve[hull[i]].y));
					int dis1 = point_y1 - approx_poly_curve[hull[i]].y;
					if(dis_point1 > SEGMENT_POINT1_DISTANCE && dis1 >= 0) 
					{
						convex_assist_above_point1++;
					}
					convex_number_above_point1++;
				}

				/*统计在中心点2以上凸点的个数*/
				if(approx_poly_curve[hull[i]].y <= point_y2)   
				{
					/*统计凸点与中心点1的y轴距离*/
					long dis_point2 = abs(long(point_y2 - approx_poly_curve[hull[i]].y));
					int dis2 = point_y2 - approx_poly_curve[hull[i]].y;
					if(dis_point2 > SEGMENT_POINT2_DISTANCE && dis2 >= 0) 
					{
						convex_assist_above_point2++;
					}
					convex_number_above_point2++;
				}
			}

			//            /*对求出的多边形拟合曲线求出凹陷集*/
			std::vector<Vec4i> convexity_defects;
			if(Mat(approx_poly_curve).checkVector(2, CV_32S) > 3)
				convexityDefects(approx_poly_curve, Mat(hull), convexity_defects);
			for(int i = 0; i < convexity_defects.size(); i++)
			{
				circle(hand_recognition_image, approx_poly_curve[convexity_defects[i][2]] , 2, Scalar(0, 0, 255), 2, 8);

				/*统计在中心点1以上凹陷点的个数*/
				if(approx_poly_curve[convexity_defects[i][2]].y <= point_y1)
					concave_number_above_point1++;

				/*统计在中心点2以上凹陷点的个数*/
				if(approx_poly_curve[convexity_defects[i][2]].y <= point_y2)
					concave_number_above_point2++;
			}
		}
	}

	/**画出手势的中心点**/
	for(auto itUser = openni.hand_points_.cbegin(); itUser != openni.hand_points_.cend(); ++itUser) 
	{
		circle(hand_recognition_image, Point(itUser->second.X, itUser->second.Y), 3, Scalar(0, 255, 255), 3, 8);
		circle(hand_recognition_image, Point(itUser->second.X, itUser->second.Y + 25), 3, Scalar(255, 0, 255), 3, 8);
	}


	/*手势数字0~5的识别*/
	//"0"的识别
	if((convex_assist_above_point1 ==0 && convex_number_above_point2 >= 2 && convex_number_above_point2 <= 3 &&concave_number_above_point2 <= 1 && concave_number_above_point1 <= 1) || (concave_number_above_point1 ==0|| concave_number_above_point2 == 0) && recognition_flag == true)
		number_result = 0;
	//"1"的识别
	if(convex_assist_above_point1 ==1 && convex_number_above_point1 >=1  && convex_number_above_point1 <=2 &&convex_number_above_point2 >=2 && convex_assist_above_point2 == 1)
		number_result = 1;
	//"2"的识别
	if(convex_number_above_point1 == 2 && concave_number_above_point1 == 1 && convex_assist_above_point2 == 2/*convex_assist_above_point1 <=1*/ && concave_number_above_point2 == 1)
		number_result = 2;
	//"3"的识别
	if(convex_number_above_point1 == 3 && concave_number_above_point1 <= 3 &&concave_number_above_point1 >=1 && convex_number_above_point2 >= 3 && convex_number_above_point2 <= 4 &&convex_assist_above_point2 == 3)
		number_result = 3;
	//"4"的识别
	if(convex_number_above_point1 == 4 && concave_number_above_point1 <=3 && concave_number_above_point1 >=2 &&convex_number_above_point2 == 4)
		number_result = 4;
	//"5"的识别
	if(convex_number_above_point1 >=4 && convex_number_above_point2 == 5 && concave_number_above_point2 >= 3 &&convex_number_above_point2 >= 4)
		number_result = 5;
	if(number_result !=0 && number_result != 1  && number_result != 2 && number_result != 3 && number_result != 4 && number_result != 5)
		number_result = -1;

	/*在手势识别图上显示匹配的数字*/
	std::stringstream number_str;
	number_str << number_result;
	putText(hand_recognition_image, "Match: ", Point(0, 60), 4, 1, Scalar(0, 255, 0), 2, 0 );
	if(number_result == -1)
		putText(hand_recognition_image, " ", Point(120, 60), 4, 2, Scalar(255, 0 ,0), 2, 0);
	else
		putText(hand_recognition_image, number_str.str(), Point(150, 60), 4, 2, Scalar(255, 0 ,0), 2, 0);

	Mat hand_segment_rgb;
	cvtColor(hand_segment,hand_segment_rgb,CV_BGR2RGB);

	qHandImage=QImage((const unsigned char*)hand_segment_rgb.data,hand_segment_rgb.cols,hand_segment_rgb.rows,hand_segment_rgb.channels()*hand_segment_rgb.cols,QImage::Format_RGB888);
	qScaledHandImage = qHandImage.scaled(ui.handView->width()-5,ui.handView->height()-5,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	handItem->setPixmap(QPixmap::fromImage(qScaledHandImage));

	qMatchImage=QImage((const unsigned char*)hand_recognition_image.data,hand_recognition_image.cols,hand_recognition_image.rows,hand_recognition_image.channels()*hand_recognition_image.cols,QImage::Format_RGB888);
	qScaledMatchImage = qMatchImage.scaled(ui.matchView->width()-5,ui.matchView->height()-5,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	matchItem->setPixmap(QPixmap::fromImage(qScaledMatchImage));

	convex_number_above_point1 = 0;
	convex_number_above_point2 = 0;
	concave_number_above_point1 = 0;
	concave_number_above_point2 = 0;
	convex_assist_above_point1 = 0;
	convex_assist_above_point2 = 0;
	number_result = -1;
	recognition_flag = false;
	number_str.clear();



}

void MainWindow::on_actionStop_triggered()
{
	openni.Stop();
	setStatusTip("Stop Kinect...");
	cout<<"Stop Kinect..."<<endl;
}

void MainWindow::on_actionAboutQt_triggered()
{
	qApp->aboutQt();
}

void MainWindow::on_actionAboutSDK_triggered()
{
	QMessageBox::about(this, tr("About Gesture Recognition"),
		tr("<h2>Gesture Recognition</h2>"
		"<p>Created by Lv Dongqing,Yang Haozhe,Yu Wenbo,Zhou Hanyue,Hou Yuechao."
		"<p>Gesture Recognition is a small application that recognizes the gestures from 1 to 5."
		"<p>Gesture Recognition is programmed with Qt,OpenNI,OpenCV using Microsoft Kinect.")
		);
}
