#include "CefAppImp.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

namespace {

    // When using the Views framework this object provides the delegate
    // implementation for the CefWindow that hosts the Views-based browser.
    class CefAppWindowDelegate : public CefWindowDelegate {
    public:
        explicit CefAppWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
            : browser_view_(browser_view) {}

        void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
            // Add the browser view and show the window.
            window->AddChildView(browser_view_);
            window->Show();

            // Give keyboard focus to the browser view.
            browser_view_->RequestFocus();
        }

        void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
            browser_view_ = NULL;
        }

        bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
            // Allow the window to close if the browser says it's OK.
            CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
            if (browser)
                return browser->GetHost()->TryCloseBrowser();
            return true;
        }

        CefSize GetPreferredSize(CefRefPtr<CefView> view) OVERRIDE {
            return CefSize(800, 600);
        }

    private:
        CefRefPtr<CefBrowserView> browser_view_;

        IMPLEMENT_REFCOUNTING(CefAppWindowDelegate);
        DISALLOW_COPY_AND_ASSIGN(CefAppWindowDelegate);
    };

}  // namespace

CefAppImp::CefAppImp(){}

void CefAppImp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();

    CefRefPtr<CefCommandLine> command_line =
        CefCommandLine::GetGlobalCommandLine();

    ////  const bool use_transparent_painting =
    ////      use_windowless_rendering_ &&
    //      command_line_->HasSwitch(switches::kTransparentPaintingEnabled);

#if defined(OS_WIN) || defined(OS_LINUX)
  // Create the browser using the Views framework if "--use-views" is specified
  // via the command-line. Otherwise, create the browser using the native
  // platform framework. The Views framework is currently only supported on
  // Windows and Linux.
    const bool use_views = command_line->HasSwitch("use-views");
#else
    const bool use_views = false;
#endif

    //  // SimpleHandler implements browser-level callbacks.
    //  CefRefPtr<SimpleHandler> handler(new SimpleHandler(use_views));

    //  // Specify CEF browser settings here.
    //  CefBrowserSettings browser_settings;

    //  std::string url;

    //  // Check if a "--url=" value was provided via the command-line. If so, use
    //  // that instead of the default URL.
    //  url = command_line->GetSwitchValue("url");
    //  if (url.empty())
    //    url = "http://www.google.com";

    //  if (use_views) {
    //    // Create the BrowserView.
    //    CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
    //        handler, url, browser_settings, NULL, NULL, NULL);

    //    // Create the Window. It will show itself after creation.
    //    CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view));
    //  } else {
    //    // Information used when creating the native window.
    //    CefWindowInfo window_info;

    //#if defined(OS_WIN)
    //    // On Windows we need to specify certain flags that will be passed to
    //    // CreateWindowEx().
    //    window_info.SetAsPopup(NULL, "cefsimple");
    //#endif

    //    // Create the first browser window.
    //    CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
    //                                  NULL, NULL);
    //  }
}


