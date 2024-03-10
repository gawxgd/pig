#include "app.h"
#include <stdexcept>
#include <time.h>
//#include "resource.h"

#define ID_TIMER1 1
#define ID_TIMER2 2
#define F4 2
#define ALTC 3

std::wstring const app::s_class_name{ L"app window" };
std::wstring const app::screen_class_name{ L"screen window" };

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
		
		if (wparam == ID_TIMER2 && pulse)
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
			SetWindowPos(m_main, NULL, pos.x - change / 4 , pos.y - change / 4, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
		break;
		case WM_HOTKEY:
			switch (static_cast<int>(wparam))
			{
				case F4:
					ShowErrorMessageBox(0);

					//PostQuitMessage(EXIT_SUCCESS);
					break;
				case ALTC:
					pulse = false;
					ShowErrorMessageBox(0);
					break;

			}
			break;

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
	pulse = true;
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
void app::RegisterHot()
{
	if (RegisterHotKey(NULL, ALTC,MOD_ALT | MOD_SHIFT, 'C'))
	{
		// Hotkey registered successfully
	}
	else
	{
		ShowErrorMessageBox(GetLastError());
	}
	if (RegisterHotKey(NULL, F4, MOD_ALT | MOD_SHIFT, VK_F4))
	{
		// Hotkey registered successfully
	}
	else
	{
		ShowErrorMessageBox(GetLastError());
	}
}
void app::ShowErrorMessageBox(DWORD errorCode)
{
	LPVOID errorMsgBuffer = nullptr;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorCode,
		0, // Default language
		reinterpret_cast<LPWSTR>(&errorMsgBuffer),
		0,
		NULL
	);

	// Display error message in MessageBox
	WCHAR message[256];
	swprintf_s(message, L"Failed with error %d:\n%s", errorCode, errorMsgBuffer ? static_cast<LPCWSTR>(errorMsgBuffer) : L"Unknown error");

	MessageBox(
		NULL,
		message,
		L"Error",
		MB_ICONERROR | MB_OK
	);

	// Free the buffer allocated by FormatMessage
	LocalFree(errorMsgBuffer);
}
app::app(HINSTANCE instance) : m_instance{ instance }
{
	register_class();
	registerScreen_class();
	/*screenWindow sc{ instance };
	screen = sc;
	screen.register_class();
	screen.create_screen_window();*/
	DWORD main_style = WS_OVERLAPPED | WS_POPUP;
	m_main = create_window(main_style);
	create_screen_window();
}

int app::run(int show_command)
{
	ShowWindow(m_main, show_command);
	ShowWindow(m_screen, show_command);
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
bool app::registerScreen_class()
{
	WNDCLASSEXW desc{};
	if (GetClassInfoExW(m_instance, screen_class_name.c_str(), &desc) != 0)
		return true;
	desc = {
	.cbSize = sizeof(WNDCLASSEXW),
	.lpfnWndProc = app::window_proc_static,
	.hInstance = m_instance,

	.hCursor = LoadCursorW(nullptr,L"IDC_ARROW"),
	.hbrBackground = CreateSolidBrush(RGB(255,255,255)),
	.lpszMenuName = nullptr,
	.lpszClassName = screen_class_name.c_str(),
	};
	return RegisterClassExW(&desc) != 0;
}

void app::create_screen_window()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HWND window = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT,screen_class_name.c_str(), L"screen", WS_OVERLAPPED | WS_POPUP, 0, 0, screenWidth, screenHeight, nullptr, nullptr, m_instance, this);
	SetLayeredWindowAttributes(window, 0, 255 * 20 / 100, LWA_ALPHA);
	m_screen = window;
}