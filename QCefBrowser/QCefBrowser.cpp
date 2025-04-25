#include "QHMCefBrowser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "CefBrowser.h"

QHMCefBrowser::QHMCefBrowser(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground);
//	ui.widget->setAutoFillBackground(true);

	hide();
	//����Web�˵���Ϣ
	connect(ui.widget_web, &QCefWebView::recvRenderMsg, this, &QHMCefBrowser::parseWebMessage, Qt::UniqueConnection);
	//ҳ������������ʾ
	connect(ui.widget_web, &QCefWebView::loadFinished, this, [=](bool status) {
		if (status) {
			show();
			raise();
		}
	}, Qt::UniqueConnection);

	//connect(ui.widget_web, &QCefWebView::exitProcessMessage, this, [=]() {
		//delete this;
		//exit(EXIT_FAILURE);
	//}, Qt::UniqueConnection);
	//����ҳ��͸��
//	ui.webEngineView->page()->setBackgroundColor(Qt::transparent);
	//�رհ�ť
	connect(ui.pushButton, &QPushButton::clicked, this, &QWidget::close);
	//Toast����
	//Toast״̬�뷢�͵�H5��
	//connect(vLiveMgr, &VLiveMgr::sigSendToastToJs, this, [=](const QString &code) {
	//	ui.webEngineView->sendToWebPage(code);
	//});
}

QHMCefBrowser::~QHMCefBrowser()
{
}

void QHMCefBrowser::openUrl(const QString & url)
{
	ui.widget_web->loadUrl(url);
	//show();
}

void QHMCefBrowser::setToast(const QString & text)
{
	//m_labelToast.setToast(text);
}

void QHMCefBrowser::mouseMoveEvent(QMouseEvent * e)
{
	if ((e->buttons() & Qt::LeftButton) && m_bMove) {
		move(e->globalPos() - m_point);//�ƶ�����
	}
}

void QHMCefBrowser::mousePressEvent(QMouseEvent * e)
{
	//����¼��а�������ס���������
	if (e->button() == Qt::LeftButton) {
		m_bMove = true;
		m_point = e->globalPos() - frameGeometry().topLeft();
	}
}

void QHMCefBrowser::mouseReleaseEvent(QMouseEvent * e)
{
	m_bMove = false;
}

void QHMCefBrowser::closeEvent(QCloseEvent * event)
{
	//QCefProcessExit();
	ui.widget_web->closeBrowser(event);
	return __super::closeEvent(event);
}

void QHMCefBrowser::showEvent(QShowEvent *event)
{
	return __super::showEvent(event);
}

void QHMCefBrowser::resizeEvent(QResizeEvent *event)
{
	return __super::resizeEvent(event);
}

void QHMCefBrowser::parseWebMessage(const QString & message)
{
	//qInfo() << QStringLiteral("�ҳ����Ϣ:") << message;
	// ����json��ʽ
	QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());
	if (jsonDoc.isNull()) {
		return;
	}
	if (jsonDoc.isObject()) {
		QJsonObject obj = jsonDoc.object();
		int action = obj.value("action").toInt(-1);
		switch (action)
		{
			/*	case E_WebActionClose: {
					close();
					break;
				}*/
		default:
			break;
		}
	}
}
