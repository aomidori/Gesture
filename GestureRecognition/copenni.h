#ifndef COPENNI_H
#define COPENNI_H

#include <XnCppWrapper.h>
#include <XnCyclicStackT.h>
#include <XnHashT.h>
#include <XnListT.h>
#include <iostream>
#include <map>
#include <vector>


using namespace xn;
using namespace std;

class COpenNI
{
public:
	COpenNI();
	~COpenNI();

	/*OpenNI的内部初始化，属性设置*/
	bool Initial();

	/*启动OpenNI读取Kinect数据*/
	bool Start();

	/*更新OpenNI读取到的数据*/
	bool UpdateData();

	void Stop();

	/*得到色彩图像的node*/
	ImageGenerator& getImageGenerator();

	/*得到深度图像的node*/
	DepthGenerator& getDepthGenerator();

	/*得到人体的node*/
	UserGenerator& getUserGenerator();

	/*得到手势姿势的node*/
	GestureGenerator& getGestureGenerator();

	/*得到手部的node*/
	HandsGenerator& getHandGenerator();

	DepthMetaData depth_metadata_;   //返回深度图像数据
	ImageMetaData image_metadata_;   //返回彩色图像数据
	std::map<XnUserID, XnPoint3D> hand_points_;  //为了存储不同手的实时点而设置的
	std::map< XnUserID, vector<XnPoint3D> > hands_track_points_; //为了绘画后面不同手部的跟踪轨迹而设定的

private:
	/*该函数返回真代表出现了错误，返回假代表正确*/
	bool CheckError(const char* error);

	/*表示有人体进入的回调函数*/
	static void XN_CALLBACK_TYPE CBNewUser(UserGenerator &generator, XnUserID user, void *p_cookie);

	/*表示骨骼校正完成的回调函数*/
	static void XN_CALLBACK_TYPE CBCalibrationComplete(SkeletonCapability &skeleton,XnUserID user, XnCalibrationStatus calibration_error, void *p_cookie);

	/*表示某个手势动作已经完成检测的回调函数*/
	static void XN_CALLBACK_TYPE  CBGestureRecognized(xn::GestureGenerator &generator, const XnChar *strGesture,const XnPoint3D *pIDPosition, const XnPoint3D *pEndPosition,void *pCookie);

	/*表示检测到某个手势开始的回调函数*/
	static void XN_CALLBACK_TYPE CBGestureProgress(xn::GestureGenerator &generator, const XnChar *strGesture,const XnPoint3D *pPosition, XnFloat fProgress, void *pCookie);

	/*手部开始建立的回调函数*/
	static void XN_CALLBACK_TYPE HandCreate(HandsGenerator& rHands, XnUserID xUID, const XnPoint3D* pPosition,XnFloat fTime, void* pCookie);

	/*手部开始更新的回调函数*/
	static void XN_CALLBACK_TYPE HandUpdate(HandsGenerator& rHands, XnUserID xUID, const XnPoint3D* pPosition, XnFloat fTime,void* pCookie);

	/*手部销毁的回调函数*/
	static void XN_CALLBACK_TYPE HandDestroy(HandsGenerator& rHands, XnUserID xUID, XnFloat fTime, void* pCookie);

	XnStatus status_;
	Context context_;
	XnMapOutputMode xmode_;
	UserGenerator user_generator_;
	ImageGenerator  image_generator_;
	DepthGenerator  depth_generator_;
	GestureGenerator gesture_generator_;
	HandsGenerator  hand_generator_;
};

#endif // COPENNI_H

