#pragma once
#include <windows.h>
#include <string>
#include <ctime>
#include "app.h"
#define pocisk_count 5
class screenWindow
{
public:
	bool register_class();
	static std::wstring const s_class_name;
	//app main_app;
	HINSTANCE instance;
	HWND m_screen;
	void create_screen_window();
	

	screenWindow(HINSTANCE instance);
	
};