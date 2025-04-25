#ifndef CEFOSRHANDLERIMP_H
#define CEFOSRHANDLERIMP_H

#include "include/cef_client.h"
#include "include/base/cef_lock.h"

#include <list>
#include <QImage>
#include <QPainter>
#include <WinUser.h>
#include <QDebug>
#include <QObject>

//������Ⱦ�¼��У�һ���� CefRenderHandler ����Ӧ�Ľӿڣ�Ҫô�� QWidget �Ͻ��¼����ݸ� Cef��Ҫô���� CefRenderHandler ����Ӧ�Ľӿ��л�ȡ�仯������ݣ����� postEvent �����Զ����¼����������ݴ��ݸ� QWidget��������Ӧ����

class CefOsrHandlerImp : public CefClient,
    public CefLifeSpanHandler,
    public CefLoadHandler,
    public CefFocusHandler,
    public CefDisplayHandler,
    public CefRenderHandler //������Ⱦ
{
public:
    explicit CefOsrHandlerImp(bool use_views);
    ~CefOsrHandlerImp() OVERRIDE;

	void setWinHandle(HWND hwnd);
	void setReceiver(QObject* receiver);

    // Provide access to the single global instance of this object.
    static CefOsrHandlerImp* GetInstance();

    // CefClient methods:
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE { return this; }

    virtual CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE { return this; }

    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }

    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this; } //������Ⱦ

    // CefLifeSpanHandler methods:
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

    // CefLoadHandler methods:
    virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        ErrorCode errorCode,
        const CefString& errorText,
        const CefString& failedUrl) OVERRIDE;

    // CefFocusHandler methods
  //  virtual bool OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source) OVERRIDE;

    // Request that all existing browser windows close.
    void CloseAllBrowsers(bool force_close);

    bool IsClosing() const { return is_closing_; }

    std::list<CefRefPtr<CefBrowser> > getCefBrowerList();

    //CefRenderHandler methods
    virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;

    virtual void OnPaint(CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList& dirtyRects,
        const void* buffer,
        int width,
        int height) OVERRIDE;

	// CefDisplayHandler methods Called when the browser's cursor has changed
    virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
        CefCursorHandle cursor,
        CursorType type,
        const CefCursorInfo& custom_cursor_info) OVERRIDE;

    // Called to retrieve the translation from view coordinates to actual screen
    // coordinates. Return true if the screen coordinates were provided.
    virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
        int viewX,
        int viewY,
        int& screenX,
        int& screenY) OVERRIDE;

private:
    // True if the application is using the Views framework.
    const bool use_views_;

    // List of existing browser windows. Only accessed on the CEF UI thread.
    typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
    BrowserList browser_list_;

    bool is_closing_;

    // True if mouse cursor change is disabled.
    bool m_bMouseCursorChangeDisabled;

    HWND m_hwnd;
    QObject* m_receiver;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(CefOsrHandlerImp);
};

#endif // CEFOSRHANDLERIMP_H