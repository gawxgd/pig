#pragma once

#include <deque>
#include <windows.h>
#include <string>
#include <ctime>
#include "screenWindow.h"
#include <shellapi.h>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

#define MAX_KEYS 10
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
	int circleSize;
	int circleMax;
	int circleMin;
	RECT textRect = { 0,0,200,200 };
	bool help;
	
	NOTIFYICONDATA nid;
	void create_notify();
	void show_tray_menu();
	POINT Mousepoint;
	void chooseColor();
	CHOOSECOLOR cc;                 
	COLORREF acrCustClr[16];
	HBRUSH hbrush;         // brush handle
	DWORD rgbCurrent;

	void openConfigIni();
	void LoadConfigIni();
	std::wstring helpString;

	std::deque<std::wstring> keyQueue;
	HHOOK hHook = NULL;
	bool hookDisplay;
	RECT keyRect;
	void InstallKeyboardHook();
	void UninstallKeyboardHook();
	static app* s_appInstance;
	
	LRESULT HandleKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	
	app(HINSTANCE instance);
	int run(int show_command);
	~app() {}
};