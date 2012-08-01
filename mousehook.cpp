#include <windows.h>
#include <winable.h>
#include <iostream>

#pragma data_seg("shared")
#pragma comment(linker, "/section:shared, rws")
// shared_data
#pragma data_seg()


#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif


/*==============================================
		
/==============================================*/

HHOOK hhook = NULL;
UINT time_id = 0;
UINT time2_id = 0;
DWORD down_type = 0; //left or right
DWORD up_type = 0;
bool user_mouse_event = false;

int delay = 1000;
int interval = 300;
#define USER_MOUSE_EVENT 123456789

EXPORT LRESULT CALLBACK mouse_hook_callback(int nCode, WPARAM wParam, LPARAM lParam);
EXPORT void set_hhook(HHOOK h);
EXPORT void set_interval(int i);
EXPORT void set_delay(int i);

VOID CALLBACK DelayTimerProc(HWND hwnd, UINT uMsg, UINT iTimerID, DWORD dwTime);
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT iTimerID, DWORD dwTime);
void kill_timer(int i);
void click(bool down = true, bool up = true);

/*==============================================
		main
/==============================================*/

int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved){
	return TRUE;
}


/*==============================================
		hook callback
/==============================================*/

EXPORT LRESULT CALLBACK mouse_hook_callback(int nCode, WPARAM wParam, LPARAM lParam){
	if (nCode < 0){ return CallNextHookEx(hhook, nCode, wParam, lParam); }
	
	switch (wParam){
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		std::cout<<"down!"<<std::endl;

		if (user_mouse_event){ break; }

		if (wParam == WM_LBUTTONDOWN){ 
			down_type = MOUSEEVENTF_LEFTDOWN;
			up_type = MOUSEEVENTF_LEFTUP;
		} else {
			down_type = MOUSEEVENTF_RIGHTDOWN;
			up_type = MOUSEEVENTF_RIGHTUP;
		}

		if (time_id == 0){ time_id = SetTimer(NULL, 0, delay, DelayTimerProc); }
		break;

	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		std::cout<<"up!"<<std::endl;

		if (user_mouse_event){ break; }
		//if (HIWORD(GetAsyncKeyState(VK_LBUTTON)) && HIWORD(GetAsyncKeyState(VK_RBUTTON))) {
			kill_timer(time_id);
			kill_timer(time2_id);
			time_id = time2_id = 0;
		//}
		break;
	}

	return CallNextHookEx(hhook, nCode, wParam, lParam);
}

/*==============================================
		set data
/==============================================*/
EXPORT void set_hhook(HHOOK h){
	hhook = h;
}

EXPORT void set_interval(int i){
	interval = i;
}

EXPORT void set_delay(int i){
	delay = i;
}

/*==============================================
   other
/==============================================*/

VOID CALLBACK DelayTimerProc(HWND hwnd, UINT uMsg, UINT iTimerID, DWORD dwTime){
	if (time2_id == 0){
		click(false);
		time2_id = SetTimer(NULL, 0, interval, TimerProc);
	}
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT iTimerID, DWORD dwTime){
	click();
}

void kill_timer(int id){
	if (id != 0){ KillTimer(NULL, id); }
}

//=================================

void click(bool down, bool up){
	user_mouse_event = true;
	if (down) { mouse_event(down_type, 0, 0, 0, GetMessageExtraInfo()); }
	if (up) { mouse_event(up_type, 0, 0, 0, GetMessageExtraInfo()); }
	user_mouse_event = false;
}
