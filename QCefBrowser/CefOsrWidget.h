#ifndef CEFOSRWIDGET_H
#define CEFOSRWIDGET_H

#include "CefOsrHandlerImp.h"

#include <QWidget>
#include "ui_CefOsrWidget.h"

class CefOsrWidget : public QWidget
{
	Q_OBJECT

public:
    CefOsrWidget(QWidget* parent = nullptr);
    ~CefOsrWidget() override;

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    bool event(QEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    CefRefPtr<CefBrowser> getBrowser();

private:
    //��Qt����갴ťת����Cef�е���갴�������������м����Ҽ���
    cef_mouse_button_type_t toCefMouseButton(Qt::MouseButton mouseButton);
	//��Qt�ļ����¼�ת����Cef�еļ����¼�
    CefKeyEvent toCefKeyEvent(QKeyEvent* event);

private:
    Ui::vCefOsrWidget ui;

    CefRefPtr<CefOsrHandlerImp> m_browserHander;
    QImage m_image;
};

#endif // CEFOSRWIDGET_H
