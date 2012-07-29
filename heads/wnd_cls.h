
#ifndef __WND_CLS__H__
#define __WND_CLS__H__

#include <windows.h>
#include "./event_callback_list.h"

namespace XJZ{
	#pragma pack(push, 1)
	struct THUNK
	{
		DWORD	dw_mov_esp;
		DWORD dw_this;
		BYTE	b_jmp;
		DWORD	dw_real_proc;

		void init(DWORD, void *);
	};
	#pragma pack(pop)

	//=======================================================================

	class WndCls
	{
	public:
		THUNK thunk;
		HWND 	hwnd;
		MSG 	msg;
		HINSTANCE hInstance;
		int iCmdShow;
		void* data;

		char const *cls_name;
		char const *caption;

		WndCls(HINSTANCE &, int, char const *title = "Window App");
		~WndCls();
		bool init_wndclass(WNDCLASS * wndcls = NULL);
		bool create_window(
			DWORD dwExStyle = WS_EX_OVERLAPPEDWINDOW, DWORD style = WS_OVERLAPPEDWINDOW,
			HWND parent_hwnd = NULL, HMENU hmenu = NULL);
		bool message_loop();

		bool bind(UINT, EVENT_CALLBACK);
		bool unbind(UINT, EVENT_CALLBACK);

		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK InitProc(HWND, UINT, WPARAM, LPARAM);

	private:
		bool reg_cls();
		char* get_cls_name();

		WNDCLASS wndclass;
		bool register_class;
		static int _window_sum;

		EventManager *event_manager;
	};
}

#endif
