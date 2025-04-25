#include "mainwindow.h"
#include <QApplication>
#include "CefBrowser.h"
#include "CefOsrWidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//������Ⱦ����ʾ��
	/*QCefProcessStartOsr();
    CefOsrWidget w;
    w.show();*/

	//CEF�����ڲ���ʾ��
	QCefProcessStart();
	MainWindow w;
	w.show();

	QCefMessageLoop();

	int qt_exit_code = a.exec();
	
	QCefProcessExit();
	return qt_exit_code;
}
