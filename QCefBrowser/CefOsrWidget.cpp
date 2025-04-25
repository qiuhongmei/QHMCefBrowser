#include "CefOsrWidget.h"

#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <events.h>

CefOsrWidget::CefOsrWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    //Cef������Ⱦ�Ļ���������WA_TranslucentBackgroundҲû����Ⱦ����
    setAttribute(Qt::WA_TranslucentBackground);
    //setWindowFlags(Qt::FramelessWindowHint);

    // ��ʼ�����ڴ�СΪ1200x800����
    resize(1200, 800);

	//��Ӧ������Ⱦ������¼�
    setMouseTracking(true);

    HWND wnd = (HWND)this->winId();
    QString indexUrl = "https://www.bilibili.com/video/BV1pA411H7vg?from=search&seid=10147493024578840911";
    //������Ի��ɱ��ص��������ر�����ҳ�ļ�����ʽΪ��file://����·���е�xx.html
    //����"file://" + QCoreApplication::applicationDirPath() + "/index.html"

    CefWindowInfo cefWndInfo;
    cefWndInfo.SetAsWindowless(wnd);

    CefBrowserSettings cefBrowSetting;
    //    cefBrowSetting.background_color = 180;
    cefBrowSetting.windowless_frame_rate = 60; //����֡�ʣ�Ĭ��ֵ��30

    m_browserHander = CefRefPtr<CefOsrHandlerImp>(new CefOsrHandlerImp(false));
    m_browserHander->setWinHandle(wnd);
    m_browserHander->setReceiver(this);

    //ע���������첽����
    bool isCreated = CefBrowserHost::CreateBrowser(cefWndInfo, m_browserHander, CefString(indexUrl.toStdWString()), cefBrowSetting, nullptr);
    if (!isCreated)
    {
        QMessageBox::information(nullptr, "warning", "brower create failed!");
    }
}

CefOsrWidget::~CefOsrWidget()
{

}

void CefOsrWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    CefRefPtr<CefBrowser> pBrower = getBrowser();
    if (pBrower)
    {
        pBrower->GetHost()->WasResized();
        pBrower->GetHost()->SetMouseCursorChangeDisabled(false);
    }
}

void CefOsrWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
        QPainter painter(this);

    QRect rect = this->rect();

    //    painter.setOpacity(0.5); //������Ըı�����������͸���ȣ�������������Ϊ0.5

    painter.drawImage(QRect(rect.left(), rect.top(), rect.width(), rect.height()), m_image, m_image.rect());
}

bool CefOsrWidget::event(QEvent* event)
{
    if (event->type() == UpdateEvent::eventType)
    {
        UpdateEvent* updateEvent = dynamic_cast<UpdateEvent*>(event);
        m_image = updateEvent->m_image;
        this->update();

        return true;
    }
    else if (event->type() == ChangeCursorEvent::eventType)
    {
        ChangeCursorEvent* changeCursorEvent = dynamic_cast<ChangeCursorEvent*>(event);

        cef_cursor_type_t cursorShape = changeCursorEvent->shape;
        Qt::CursorShape qtCursorShape = Qt::ArrowCursor;

        //TODO ��Ҫ�Զ������������ʽ�����ڴ˴��޸�
        switch (cursorShape)
        {
        case CT_POINTER:
            qtCursorShape = Qt::ArrowCursor;
            break;

        case CT_CROSS:
            qtCursorShape = Qt::CrossCursor;
            break;

        case CT_HAND:
            qtCursorShape = Qt::PointingHandCursor;
            break;

        case CT_IBEAM:
            qtCursorShape = Qt::IBeamCursor;
            break;

        case CT_WAIT:
            qtCursorShape = Qt::WaitCursor;
            break;

        default:
            break;
        }

        this->setCursor(qtCursorShape);

        return true;
    }
    else if (event->type() == ReloadBrowerEvent::eventType)
    {
        CefRefPtr<CefBrowser> pBrower = getBrowser();
        if (pBrower)
        {
            pBrower->Reload();
        }
    }
    return QWidget::event(event);
}

void CefOsrWidget::wheelEvent(QWheelEvent* event)
{
    QPoint pos = event->pos();
    CefRefPtr<CefBrowser> pBrower = getBrowser();
    if (pBrower)
    {
        CefMouseEvent cefMouseEvent;
        cefMouseEvent.x = pos.x();
        cefMouseEvent.y = pos.y();
        pBrower->GetHost()->SendMouseWheelEvent(cefMouseEvent, 0, event->delta()); //��Cef����һ�������¼�
    }

    //��Ϊ���ش��ݸ��²㣬������������ע�͵�
    //QWidget::wheelEvent(event);
}

void CefOsrWidget::mousePressEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    CefRefPtr<CefBrowser> pBrower = getBrowser();
    if (pBrower)
    {
        CefMouseEvent cefMouseEvent;
        cefMouseEvent.x = pos.x();
        cefMouseEvent.y = pos.y();

        cef_mouse_button_type_t cefMouseButton = toCefMouseButton(event->button());

        pBrower->GetHost()->SendMouseClickEvent(cefMouseEvent, cefMouseButton, false, 1);
    }
    //QWidget::mousePressEvent(event);
}

void CefOsrWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    CefRefPtr<CefBrowser> pBrower = getBrowser();
    if (pBrower)
    {
        CefMouseEvent cefMouseEvent;
        cefMouseEvent.x = pos.x();
        cefMouseEvent.y = pos.y();

        cef_mouse_button_type_t cefMouseButton = toCefMouseButton(event->button());
        pBrower->GetHost()->SendMouseClickEvent(cefMouseEvent, cefMouseButton, true, 1);
    }
    //QWidget::mouseReleaseEvent(event);
}

void CefOsrWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    CefRefPtr<CefBrowser> pBrower = getBrowser();
    if (pBrower)
    {
        CefMouseEvent cefMouseEvent;
        cefMouseEvent.x = pos.x();
        cefMouseEvent.y = pos.y();

        cef_mouse_button_type_t cefMouseButton = toCefMouseButton(event->button());
        pBrower->GetHost()->SendMouseClickEvent(cefMouseEvent, cefMouseButton, false, 2);
    }
    //QWidget::mouseDoubleClickEvent(event);
}

void CefOsrWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    CefRefPtr<CefBrowser> pBrower = getBrowser();
    if (pBrower)
    {
        CefMouseEvent cefMouseEvent;
        cefMouseEvent.x = pos.x();
        cefMouseEvent.y = pos.y();
        cefMouseEvent.modifiers = EVENTFLAG_LEFT_MOUSE_BUTTON;

        pBrower->GetHost()->SendMouseMoveEvent(cefMouseEvent, false);
    }
    //QWidget::mouseMoveEvent(event);
}

void CefOsrWidget::keyPressEvent(QKeyEvent* event)
{
    CefRefPtr<CefBrowser> pBrower = getBrowser();
    if (pBrower)
    {
        pBrower->GetHost()->SendKeyEvent(toCefKeyEvent(event));
    }

    QWidget::keyPressEvent(event);
}

CefRefPtr<CefBrowser> CefOsrWidget::getBrowser()
{
    if (!m_browserHander.get())
    {
        return nullptr;
    }

    std::list<CefRefPtr<CefBrowser> > cefBrowerList = m_browserHander->getCefBrowerList();
    if (cefBrowerList.size() == 0)
    {
        qDebug() << "cefBrowerList.size() == 0"; //��������첽�����ģ�OnAfterCreated���ڴ����ɹ�����õ�
        return nullptr;
    }

    return cefBrowerList.front();
}

cef_mouse_button_type_t CefOsrWidget::toCefMouseButton(Qt::MouseButton mouseButton)
{
    cef_mouse_button_type_t cefMouseButton = MBT_LEFT;
    switch (mouseButton)
    {
    case Qt::LeftButton:
        cefMouseButton = MBT_LEFT;
        break;

    case Qt::RightButton:
        cefMouseButton = MBT_RIGHT;
        break;

    case Qt::MiddleButton:
        cefMouseButton = MBT_MIDDLE;
        break;

    default:
        break;
    }
    return cefMouseButton;
}

CefKeyEvent CefOsrWidget::toCefKeyEvent(QKeyEvent* event)
{
    CefKeyEvent cefKeyEvent;
    cefKeyEvent.type = KEYEVENT_CHAR;

    Qt::KeyboardModifiers keyboardModifiers = event->modifiers();
    qDebug() << "keyboardModifiers:" << keyboardModifiers;

    //TODO ֻ��Shift����Ctrl��������,������λ��Ҫ�Լ�����
    switch (keyboardModifiers)
    {
    case Qt::ShiftModifier:
        cefKeyEvent.modifiers = EVENTFLAG_SHIFT_DOWN;
        cefKeyEvent.type = KEYEVENT_CHAR;  //����ǰ�����shit���Ļ�����������Ҫ��Cef֪��������ͨ��������
        break;

    case Qt::ControlModifier:
        cefKeyEvent.modifiers = EVENTFLAG_CONTROL_DOWN;
        cefKeyEvent.type = KEYEVENT_KEYDOWN;
        break;

        //    case Qt::AltModifier:
        //      cefKeyEvent.modifiers = EVENTFLAG_ALT_DOWN;
        //      cefKeyEvent.type = KEYEVENT_KEYDOWN;
        //      break;

    default:
        break;
    }

    int key = event->key();

    qDebug() << "key: " << event->key();
    qDebug() << "nativeScanCode: " << event->nativeScanCode();
    qDebug() << "nativeVirtualKey: " << event->nativeVirtualKey();

    cefKeyEvent.windows_key_code = event->key();

    if (event->key() == Qt::Key_Backspace)
    {
        qDebug() << "event->key() == Qt::Key_Backspace";
        cefKeyEvent.type = KEYEVENT_KEYDOWN;
        cefKeyEvent.windows_key_code = static_cast<int>(event->nativeVirtualKey());
    }

    if (key >= Qt::Key_F1 && key <= Qt::Key_F12)
    {
        qDebug() << "Qt::Key_F1";
        cefKeyEvent.type = KEYEVENT_KEYDOWN;
    }

    if (event->key() == Qt::Key_F5)
    {
        qDebug() << "Qt::Key_F5"; //ˢ��
        ReloadBrowerEvent* reloadBrowerEvent = new ReloadBrowerEvent();
        QCoreApplication::sendEvent(this, reloadBrowerEvent);
    }

    return cefKeyEvent;
}
