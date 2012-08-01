#include <iostream>
#include "./heads/wnd_cls.h"

bool create_callback(void* p_data, WPARAM wParam, LPARAM lParam);
bool quit_callback(void* p_data, WPARAM wParam, LPARAM lParam);

using std::cout;
using std::endl;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	XJZ::WndCls w = XJZ::WndCls(hInstance, iCmdShow, "MouseClick");

	w.bind(WM_CREATE, create_callback);
	w.bind(WM_DESTROY, quit_callback);

	w.create_window();
	w.message_loop();

	return w.msg.wParam;
}


bool create_callback(void* p_data, WPARAM wParam, LPARAM lParam){
	XJZ::WndCls* wc = (XJZ::WndCls*)(p_data);
	HWND hwnd = wc->hwnd;
	typedef void (*SET_HHOOK_FN)(HHOOK h);
	typedef void (*SET_INT_FN)(int i);

	HINSTANCE hinstDLL = LoadLibrary(TEXT("mousehook.dll"));
	cout<<"hinstDLL: "<<hinstDLL<<endl;
	HOOKPROC hkprcMouseCallback = HOOKPROC(GetProcAddress(hinstDLL, "mouse_hook_callback@12"));
	HHOOK hhook  = SetWindowsHookEx(WH_MOUSE_LL, hkprcMouseCallback, hinstDLL, 0);

	SET_HHOOK_FN(GetProcAddress(hinstDLL, "set_hhook"))(hhook);
	SET_INT_FN(GetProcAddress(hinstDLL, "set_delay"))(1000);
	SET_INT_FN(GetProcAddress(hinstDLL, "set_interval"))(300);

	SetWindowPos(hwnd, 0, 0, 0, 200, 100, SWP_NOZORDER);

	wc->data = (void*)(hhook);

	return true;
}

bool quit_callback(void* p_data, WPARAM wParam, LPARAM lParam){
	XJZ::WndCls* wc = (XJZ::WndCls*)(p_data);

	UnhookWindowsHookEx(HHOOK(wc->data));
	FreeLibrary(GetModuleHandle(TEXT("mousehook.dll")));
	PostQuitMessage(0);
	cout<<"unhook, quit"<<endl;

	return false;
}

