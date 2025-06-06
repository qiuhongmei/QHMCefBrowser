#include "mainwindow.h"
#include <QDir>
#include <QTextCodec>
#include <QFileDialog>
#include "QHMCefBrowser.h"
#include "CefOsrWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

    QString uri = QFileInfo(QCoreApplication::applicationDirPath()+"/test.html").absoluteFilePath();
    ui.widget_html->loadUrl(uri);

    connect(ui.widget_html, &QCefWebView::recvRenderMsg, this, &MainWindow::sltRecvJsMsg);
	connect(ui.pushButton_toJs, &QPushButton::clicked, this, &MainWindow::sltSendMsgToJs);
	connect(ui.pushButton_load, &QPushButton::clicked, this, &MainWindow::sltLoadUrlWed);
	connect(ui.pushButton_test, &QPushButton::clicked, this, &MainWindow::sltLoadTestWeb);
	connect(ui.pushButton_zoom, &QPushButton::clicked, this, &MainWindow::sltZoomWeb);
	connect(ui.pushButton_refresh, &QPushButton::clicked, ui.widget_web, &QCefWebView::refresh);
	connect(ui.pushButton_back, &QPushButton::clicked, ui.widget_web, &QCefWebView::back);
	connect(ui.pushButton_forward, &QPushButton::clicked, ui.widget_web, &QCefWebView::forward);
	connect(ui.pushButton_webp, &QPushButton::clicked, this, &MainWindow::sltLoadWebp);
	connect(ui.pushButton_osr, &QPushButton::clicked, this, &MainWindow::sltLoadOsr);
}

MainWindow::~MainWindow()
{
}

void MainWindow::sltRecvJsMsg(const QString &msg)
{
    ui.label_webmsg->setText(msg);
}

void MainWindow::sltSendMsgToJs()
{
	QString text = ui.lineEdit->text();
	if (!text.isEmpty()) {
		ui.widget_html->SendMsgToPage(text);
	}
}

void MainWindow::sltLoadUrlWed()
{
	QString text = ui.lineEdit_url->text();
	ui.widget_web->loadUrl(text);
}

void MainWindow::sltLoadTestWeb()
{
	m_browser = new QHMCefBrowser();
	m_browser->setAttribute(Qt::WA_DeleteOnClose);
	if (m_browser) {
		//QString url = ui->lineEdit_url->text();
		/*QString url = "https://www.bilibili.com/video/BV1pA411H7vg?from=search&seid=10147493024578840911";*/
		QString url = "file:///E:/shaw/demo/github-shaw/QHMCefBrowser/QHMCefBrowser/animation/index.html";
		m_browser->openUrl(url);
		m_browser->show();
	}
}

void MainWindow::sltZoomWeb()
{
	auto _zoom = (double(125 - 100)) / 25.0;
	ui.widget_web->ModifyZoom(_zoom);
}

void MainWindow::sltLoadWebp()
{
	auto OpenFile = QFileDialog::getOpenFileName(this,
		"Please choose webp file",
		"",
		"Image Files(*.webp *.GIF);");

	if (!OpenFile.isEmpty()) {
		ui.widget_web->loadUrl(OpenFile);
	}
}

void MainWindow::sltLoadOsr()
{
	//������Ⱦ��CefOsrWidget ����ʾ��
}

