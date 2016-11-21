TEMPLATE = app

TARGET = 


CONFIG += console 

Qt += core gui

HEADERS += copenni.h 
  

SOURCES += main.cpp \
           copenni.cpp

FORMS += 

RESOURCES += 


INCLUDEPATH +=  D:\OpenSDKs\OpenCV2.4.11\build\include \
                D:\OpenSDKs\OpenCV2.4.11\build\include\opencv \
                D:\OpenSDKs\OpenCV2.4.11\build\include\opencv2

LIBS += opencv_calib3d2411d.lib \
	opencv_contrib2411d.lib \
	opencv_core2411d.lib \
	opencv_features2d2411d.lib \
	opencv_flann2411d.lib \
	opencv_gpu2411d.lib \
	opencv_highgui2411d.lib \
	opencv_imgproc2411d.lib \
	opencv_legacy2411d.lib \
	opencv_ml2411d.lib \
	opencv_nonfree2411d.lib \
	opencv_objdetect2411d.lib \
	opencv_ocl2411d.lib \
	opencv_photo2411d.lib \
	opencv_stitching2411d.lib \
	opencv_superres2411d.lib \
	opencv_ts2411d.lib \
	opencv_video2411d.lib \
	opencv_videostab2411d.lib \

QMAKE_LIBDIR += D:\OpenSDKs\OpenCV2.4.11\build\x86\vc10\lib


INCLUDEPATH += C:\Programming\OpenNI\Include

LIBS += NiSampleExtensionModule.lib \
        NiSampleModule.lib \
        OpenNI.jni.lib \
        openNI.lib

QMAKE_LIBDIR += C:\Programming\OpenNI\Lib\


INCLUDEPATH += C:\Programming\PrimeSense\NITE\Include

LIBS += XnVCNITE_1_5_2.lib \
	XnVFeatures_1_5_2.lib \
	XnVHandGenerator_1_5_2.lib \
	XnVNITE.jni.lib \
	XnVNITE_1_5_2.lib

QMAKE_LIBDIR += C:\Programming\PrimeSense\NITE\Lib