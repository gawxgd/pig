#pragma once

#include <windows.h>
#include <string>
#include <ctime>
#include "screenWindow.h"
#include <shellapi.h>
#define pocisk_count 5
class app
{
public:
	bool register_class();
	static std::wstring const s_class_name;
	static LRESULT CALLBACK window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	HWND create_window(DWORD stykle);
	bool pulse;
	HWND m_main;
	HINSTANCE m_instance;
	int sizeCount;
	int change;
	void RegisterHot();
	void ShowErrorMessageBox(DWORD errorCode);
	//screenWindow screen;
	bool registerScreen_class();
	static std::wstring const screen_class_name;
	void create_screen_window();
	HWND m_screen;
	RECT circleRect;
	RECT textRect = { 0,0,200,200 };
	bool help;
	
	NOTIFYICONDATA nid;
	void create_notify();
	void chooseColor();
	CHOOSECOLOR cc;                 
	COLORREF acrCustClr[16];
	HBRUSH hbrush;                  // brush handle
	DWORD rgbCurrent;



	app(HINSTANCE instance);
	int run(int show_command);
	~app() {}
};