
#ifndef __EVENT_CALLBACK_LIST__H__
#define __EVENT_CALLBACK_LIST__H__

#include <windows.h>
#include "./debug.h"

namespace XJZ{
	typedef bool (*EVENT_CALLBACK)(void*, WPARAM, LPARAM);

	class EventCallback
	{
	public:
		EventCallback(UINT uMsg, int event_length = 3);
		~EventCallback();
		bool add(EVENT_CALLBACK func);
		bool remove(EVENT_CALLBACK func);
		bool exec(void* p_data, WPARAM wp, LPARAM lp);
		UINT event_id();
	private:
		bool add_length(float val = 1.5);

		int length;
		int use_length;
		bool exec_callbacks;
		UINT _event_id;
		EVENT_CALLBACK *callbacks;
	};

	class EventManager
	{
	public:
		EventManager(int sum = 15);
		~EventManager();

		bool add(UINT uMsg, EVENT_CALLBACK func);
		bool remove(UINT uMsg, EVENT_CALLBACK func);
		bool exec(UINT uMsg, void* p_data, WPARAM wp, LPARAM lp);

	private:
		bool add_length();

		EventCallback **list;
		int length;
		int use_length;
	};
}

#endif

