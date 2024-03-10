#include "screenWindow.h"
#include "app.h"
std::wstring const screenWindow::s_class_name{ L"screenWindow" };
bool screenWindow::register_class()
{
	WNDCLASSEXW desc{};
	if (GetClassInfoExW(instance, s_class_name.c_str(), &desc) != 0)
		return true;
	desc = {
	.cbSize = sizeof(WNDCLASSEXW),
	.lpfnWndProc = app::window_proc_static,
	.hInstance = instance,

	.hCursor = LoadCursorW(nullptr,L"IDC_ARROW"),
	//.hbrBackground = CreateSolidBrush(RGB(255,255,0)),
	.lpszMenuName = nullptr,
	.lpszClassName = s_class_name.c_str(),
	};
	return RegisterClassExW(&desc) != 0;
}


void screenWindow::create_screen_window()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HWND window = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT, s_class_name.c_str(), L"screen", WS_OVERLAPPED | WS_POPUP, 0, 0, screenWidth, screenHeight, nullptr, nullptr, instance, this);
	SetLayeredWindowAttributes(window, 0, 255 * 80 / 100, LWA_ALPHA);
	m_screen = window;
}

screenWindow::screenWindow(HINSTANCE instance)
{
	instance = instance;
	register_class();
}


