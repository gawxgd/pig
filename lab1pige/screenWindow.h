#pragma once
#include <windows.h>
#include <string>
#include <ctime>
#define pocisk_count 5
class screenWindow
{
private:
	bool register_class();
	static std::wstring const s_class_name;
	static LRESULT CALLBACK window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	HWND create_window(DWORD stykle);
	HWND m_main;
	HINSTANCE m_instance;
	
public:
	screenWindow(HINSTANCE instance);
	int run(int show_command);
	
};