#include "mainwindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	w.setWindowTitle("Gesture Recognition");
	w.showMaximized();
	
	return a.exec();
}
