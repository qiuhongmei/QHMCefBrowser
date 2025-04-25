

//#pragma comment(lib,"libcef")
//#ifdef NDEBUG
//#pragma comment(lib,"libcef_dll_wrapper")
//#else
//#pragma comment(lib,"libcef_dll_wrapper_d")
//#endif

#include "CefBrowser.h"
#include "CefAppImp.h"
#include "MyRenderProcessHandler.h"

int QCefProcessStart()
{
	//CefEnableHighDPISupport();
	// Provide CEF with command-line arguments.
	HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));
	CefMainArgs mainArgs(hInstance);

	void* sandbox_info = NULL;

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	CefRefPtr<MyRenderProcessHandler> app_r(new MyRenderProcessHandler);
    int exit_code_app_r = CefExecuteProcess(mainArgs, app_r.get(), sandbox_info);
    if (exit_code_app_r >= 0) {
        // The sub-process has completed so return here.
        return exit_code_app_r;
    }
	//
	// Specify CEF global settings here.
	CefSettings settings;
	settings.no_sandbox = false; //�ر�ɳ�� 

#ifdef Q_OS_WIN
	settings.multi_threaded_message_loop = true;  //���߳���Ϣѭ��
#endif

	settings.windowless_rendering_enabled = true; //����������Ⱦ
	// CefAppImp implements application-level callbacks for the browser process.
	// It will create the first browser instance in OnContextInitialized() after
	// CEF has initialized.
	CefRefPtr<CefAppImp> app(new CefAppImp);
	int exit_code_app = CefExecuteProcess(mainArgs, app.get(), nullptr);
	if (exit_code_app >= 0) {
		return exit_code_app;
	}
	// Initialize CEF.
    CefInitialize(mainArgs, settings, app_r, sandbox_info);
	//CefInitialize(mainArgs, settings, app, sandbox_info);
    return 0;
}

int QCefProcessStartOsr() 
{
	HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));

	CefMainArgs mainArgs(hInstance);
	CefRefPtr<CefAppImp> app(new CefAppImp); //CefAppʵ�֣����ڴ��������صĻص���

	int exit_code = CefExecuteProcess(mainArgs, app.get(), nullptr);
	if (exit_code >= 0) {
		return exit_code;
	}

	CefSettings settings;
	//std::string cache_path = AppGetWorkingDirectory().toStdString() + "/.cache";//�����ַ
	// CefString(&settings.cache_path) = CefString(cache_path);
	settings.multi_threaded_message_loop = true;//���߳���Ϣѭ��
	settings.log_severity = LOGSEVERITY_DISABLE;//��־
	settings.windowless_rendering_enabled = true;
	settings.no_sandbox = false;//ɳ��
	CefInitialize(mainArgs, settings, app, nullptr);

	return -1;
}

void QCefMessageLoop()
{
#ifndef Q_OS_WIN
	CefRunMessageLoop();
#endif
}


void QCefProcessExit()
{
	// Shut down CEF.
	CefShutdown();
}

