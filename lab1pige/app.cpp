﻿#include "app.h"
#include <stdexcept>
#include <time.h>
//#include "resource.h"

#define ID_TIMER1 1
#define ID_TIMER2 2

std::wstring const app::s_class_name{ L"app window" };
bool app::register_class()
{
	WNDCLASSEXW desc{};
	if (GetClassInfoExW(m_instance, s_class_name.c_str(), &desc) != 0)
		return true;
	desc = {
	.cbSize = sizeof(WNDCLASSEXW),
	.lpfnWndProc = window_proc_static,
	.hInstance = m_instance,
	
	.hCursor = LoadCursorW(nullptr,L"IDC_ARROW"),
	.hbrBackground = CreateSolidBrush(RGB(255,255,0)),
	.lpszMenuName = nullptr,
	.lpszClassName = s_class_name.c_str(),
	};
	return RegisterClassExW(&desc) != 0;
}

LRESULT app::window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	app* application = nullptr;
	if (message == WM_NCCREATE)
	{
		application = static_cast<app*>(reinterpret_cast<LPCREATESTRUCTW>(lparam)->lpCreateParams);
		SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(application));
	}
	else
	{
		application = reinterpret_cast<app*>(GetWindowLongPtrW(window, GWLP_USERDATA));
	}
	LRESULT res = application ? application->window_proc(window, message, wparam, lparam) : DefWindowProcW(window, message, wparam, lparam);
	if (message == WM_NCDESTROY)
		SetWindowLongPtrW(window, GWLP_USERDATA, 0);
	return res;
}

LRESULT app::window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	static bool dragged = false;
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wparam);
		switch (wmId)
		{
			
		default:
			return DefWindowProc(window, message, wparam, lparam);
		}
	}
	break;
	case WM_CLOSE:
		DestroyWindow(m_main);
		return 0;
	case WM_DESTROY:
		if (window == m_main)
		{
			PostQuitMessage(EXIT_SUCCESS);
		}
		return 0;
	case WM_CREATE:
		break;
	case WM_TIMER:
		
		if (wparam == ID_TIMER2)
		{
			if (sizeCount == 1)
			{
				change += 10;
				SetWindowRgn(window, CreateEllipticRgn(0, 0, change, change), TRUE);
				if(change >= 100)
					sizeCount = 0;
			}
			else if (sizeCount == 0)
			{
				change -= 10;
				SetWindowRgn(window, CreateEllipticRgn(0, 0, change, change), TRUE);
				if (change <= 50)
					sizeCount = 1;
			
			}
			
		}
		else if (wparam == ID_TIMER1)
		{

			POINT pos;
			GetCursorPos(&pos);
			SetWindowPos(m_main, NULL, pos.x - 50, pos.y - 50, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
		break;
	/*case WM_HOTKEY:
		if(wparam ==)*/
	/*case WM_MOUSEMOVE:
	
		
		int mouseX = LOWORD(lparam);
		int mouseY = HIWORD(lparam);

		
		SetWindowPos(window, NULL, mouseX - 50, mouseY - 50, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		break;*/
	

	
	}

	return DefWindowProcW(window, message, wparam, lparam);
}

HWND app::create_window(DWORD style)
{
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);
	int x = cursor_pos.x;
	int y = cursor_pos.y;
	sizeCount = 1;
	change = 100;
	HWND window = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT, s_class_name.c_str(), L"circle", style, x - 50 , y - 50 , 100, 100, nullptr, nullptr, m_instance, this);
	//SetCapture(window);
	SetTimer(window, ID_TIMER1, 5, NULL);
	SetTimer(window, ID_TIMER2, 100, NULL);
	//RegisterHot();
	SetLayeredWindowAttributes(window, 0, 255 * 80 / 100, LWA_ALPHA);
	SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowRgn(window, CreateEllipticRgn(0,0,100,100), TRUE);
	return window;
}



app::app(HINSTANCE instance) : m_instance{ instance }
{
	//score = 0;
	register_class();
	DWORD main_style = WS_OVERLAPPED | WS_POPUP;
	m_main = create_window(main_style);
}

int app::run(int show_command)
{
	ShowWindow(m_main, show_command);


	MSG msg{};
	BOOL result = TRUE;

	while ((result = GetMessageW(&msg, nullptr, 0, 0)) != 0)
	{
		if (result == -1)
			return EXIT_FAILURE;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);


	}
	return EXIT_SUCCESS;
}