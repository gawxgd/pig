#include "app.h"
#include <stdexcept>
#include <time.h>
#include "resource.h"

#define ID_TIMER1 1
#define ID_TIMER2 2
#define F4 2
#define ALTC 3
#define F12 4
#define trayID 1
#define trayMessage WM_USER + 1
#define ID_TIMER_KEY 3

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
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wparam);
		switch (wmId)
		{
		case 1:
			DestroyWindow(m_main);
			break;
		case 2: 
			openConfigIni();
			break;
		case 3:
			LoadConfigIni();
			break;
		case 4:
			chooseColor();
			break;
			
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
				circleSize += 5;
				if(circleSize >= circleMax)
					sizeCount = 0;
			}
			else if (sizeCount == 0)
			{
				circleSize -= 5;
				if (circleSize <= circleMin)
					sizeCount = 1;
			}
			InvalidateRect(m_screen, NULL, TRUE);
			UpdateWindow(m_screen);

			
		}
		else if (wparam == ID_TIMER1)
		{
			POINT pos;
			GetCursorPos(&pos);
			RECT pastRect = circleRect;
			circleRect.left = pos.x - 50;
			circleRect.top = pos.y - 50;
			circleRect.right = circleRect.left + 100;
			circleRect.bottom = circleRect.top + 100;
			InvalidateRect(m_screen, NULL, TRUE);
			UpdateWindow(m_screen);
		}
		else if (wparam == ID_TIMER_KEY)
		{
			keyQueue.clear();
			/*timerSet = false;
			KillTimer(m_screen, ID_TIMER_KEY);*/
		}
		break;
		case WM_HOTKEY:
			switch (static_cast<int>(wparam))
			{
				case F4:
					PostQuitMessage(EXIT_SUCCESS);
					break;
				case ALTC:
					if (pulse == false)
						pulse = true;
					else
						pulse = false;
					break;
				case F12:
					if (help == false)
						help = true;
					else
						help = false;
					break;
			}
			break;
		case trayMessage:

			switch (LOWORD(lparam))
			{
			case WM_RBUTTONUP:
				GetCursorPos(&Mousepoint);

				show_tray_menu();
				break;
			}
			break;
		case WM_ERASEBKGND:
			return 1;
		case WM_PAINT: 
			PAINTSTRUCT ps;
			// stackoverflow
			// create memory DC and memory bitmap where we shall do our drawing
			HDC hdc = BeginPaint(m_screen, &ps);
			HDC memDC = CreateCompatibleDC(hdc);

			// get window's client rectangle. We need this for bitmap creation.
			RECT rcClientRect;
			GetClientRect(m_screen, &rcClientRect);

			// now we can create bitmap where we shall do our drawing
			HBITMAP bmp = CreateCompatibleBitmap(hdc,
				rcClientRect.right - rcClientRect.left,
				rcClientRect.bottom - rcClientRect.top);

			// we need to save original bitmap, and select it back when we are done,
			// in order to avoid GDI leaks!
			HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);

			// now you draw your stuff in memory dc; 
			// just substitute hdc with memDC in your drawing code, 
			// like I did below:
			
			
			SelectObject(memDC, hbrush);
			SetBkMode(memDC, TRANSPARENT);
			BLENDFUNCTION blendFunction = { AC_SRC_OVER, 0, static_cast<BYTE>(128), AC_SRC_ALPHA };
			Ellipse(memDC, circleRect.left - circleSize / 2, circleRect.top - circleSize / 2, circleRect.right + circleSize / 2, circleRect.bottom + circleSize / 2);
			if (help)
			{
				SetBkMode(memDC, OPAQUE);
				SetBkColor(memDC, RGB(255, 255, 255)); 
				ExtTextOut(memDC, 0, 0, ETO_OPAQUE, &textRect, nullptr, 0, nullptr);

				SetTextColor(memDC, RGB(0, 0, 0)); 
				SetBkMode(memDC, TRANSPARENT);
				DrawText(memDC, helpString.c_str(), -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			if (hookDisplay)
			{
				SetBkMode(memDC, OPAQUE);
				SetBkColor(memDC, RGB(255, 255, 255));
				ExtTextOut(memDC, 0, 0, ETO_OPAQUE, &keyRect, nullptr, 0, nullptr);

				SetTextColor(memDC, RGB(0, 0, 0));
				SetBkMode(memDC, TRANSPARENT);
				RECT temp = keyRect;
				temp.top = keyRect.top - 100;
				int it = 0;
				bool specialKey = false;
				std::wstring specialString;
				for (const auto& key : keyQueue) 
				{
					/*if (key.length() - 1 >0 && key[key.length() - 1] == '+')
					{
						specialKey = true;
						specialString = key;
						continue;
					}*/
						
					if (it < MAX_KEYS)
					{
						if (specialKey)
						{
							specialString.append(key);
							DrawText(memDC, specialString.c_str(), -1, &temp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
							specialKey = false;
							
						}
						else
						{
							DrawText(memDC, key.c_str(), -1, &temp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
						}
						temp.top += 30;
						it++;
					}
					else
					{
						keyQueue.pop_back();
						//KillTimer(m_screen, timerID--);
						it--;
					}
					

				}
			}
			BitBlt(hdc, 0, 0, rcClientRect.right - rcClientRect.left,
				rcClientRect.bottom - rcClientRect.top, memDC, 0, 0, SRCCOPY);
			// all done, now we need to cleanup
			SelectObject(memDC, oldBmp); // select back original bitmap
			DeleteObject(bmp); // delete bitmap since it is no longer required
			DeleteDC(memDC);   // delete memory DC since it is no longer required
			//UpdateLayeredWindow(m_screen,hdc)
			EndPaint(m_screen, &ps);
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
	circleRect.left = x - 50;
	circleRect.top = y - 50;
	circleRect.right = circleRect.left + 100;
	circleRect.bottom = circleRect.top + 100;
	circleSize = 50;
	circleMax = 75;
	circleMin = 50;
	HWND window = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT, s_class_name.c_str(), L"circle", style, x - 50 , y - 50 , 100, 100, nullptr, nullptr, m_instance, this);
	SetTimer(window, ID_TIMER1, 5, NULL);
	SetTimer(window, ID_TIMER2, 100, NULL);
	//SetLayeredWindowAttributes(window, 0, 255 * 80 / 100, LWA_ALPHA);
	SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return window;
}
void app::RegisterHot()
{
	if (RegisterHotKey(m_screen, ALTC,MOD_ALT | MOD_SHIFT, 'C'))
	{
		// Hotkey registered successfully
	}
	else
	{
		ShowErrorMessageBox(GetLastError());
	}
	if (RegisterHotKey(m_screen, F4, MOD_ALT | MOD_SHIFT, VK_F4))
	{
		// Hotkey registered successfully
	}
	else
	{
		ShowErrorMessageBox(GetLastError());
	}
	if (RegisterHotKey(m_screen, F12, MOD_CONTROL, VK_F12))
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
void app::create_notify()
{
	ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = m_screen;
	nid.uID = trayID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = trayMessage;
	nid.hIcon = LoadIcon(m_instance, MAKEINTRESOURCE(IDI_ICON1));
	Shell_NotifyIcon(NIM_ADD, &nid);
}
app::app(HINSTANCE instance) : m_instance{ instance }
{
	register_class();
	registerScreen_class();
	DWORD main_style = WS_OVERLAPPED | WS_POPUP;
	m_main = create_window(main_style);
	create_screen_window();
	help = true;
	hbrush = CreateSolidBrush(RGB(255, 255, 0));
	helpString = L"Pomoc ctrl f12 wylacz";
	s_appInstance = this;

}
app* app::s_appInstance = nullptr;

void app::chooseColor()
{
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = m_screen;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	if (ChooseColor(&cc) == TRUE)
	{
		hbrush = CreateSolidBrush(cc.rgbResult);
		rgbCurrent = cc.rgbResult;
	}
}



void app::show_tray_menu()
{
	HMENU hMenu = CreatePopupMenu();
	if (hMenu)
	{
		InsertMenu(hMenu, -1, MF_BYPOSITION | MF_STRING, 1, L"Exit");
		InsertMenu(hMenu, -1, MF_BYPOSITION | MF_STRING, 2, L"Open config file");
		InsertMenu(hMenu, -1, MF_BYPOSITION | MF_STRING, 3, L"Reload config");
		InsertMenu(hMenu, -1, MF_BYPOSITION | MF_STRING, 4, L"Pick color");

		SetForegroundWindow(m_screen);
		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, Mousepoint.x, Mousepoint.y, 0,m_screen , NULL);
		
		PostMessage(m_screen, WM_NULL, 0, 0);

		DestroyMenu(hMenu);
	}
}
void app::openConfigIni()
{
	LPCWSTR Path = L"config.ini";
	WCHAR filepath[MAX_PATH];
	GetModuleFileName(NULL,filepath, MAX_PATH);
	PathRemoveFileSpec(filepath);
	PathAppend(filepath, Path);
	auto shellEror = ShellExecute(NULL, L"edit",filepath, NULL, NULL, SW_SHOW);
	if ((int)shellEror <= 32)
	{
		auto dwError = GetLastError();
		ShowErrorMessageBox(dwError);
	}
}

void app::LoadConfigIni()
{
	LPCWSTR Path = L"config.ini";
	WCHAR filepath[MAX_PATH];
	GetModuleFileName(NULL, filepath, MAX_PATH);
	PathRemoveFileSpec(filepath);
	PathAppend(filepath, Path);
	LPCWSTR section = L"Help"; 
	LPCWSTR key = L"desc"; 
	WCHAR buffer[256]; 
	DWORD bytesRead = GetPrivateProfileString(section, key, L"", buffer, ARRAYSIZE(buffer), filepath);
	helpString = std::wstring(buffer);

	if (bytesRead < 0) 
	{
		int er = GetLastError();
		ShowErrorMessageBox(er);
	}
	InvalidateRect(m_screen, nullptr, TRUE);

}

int app::run(int show_command)
{
	//ShowWindow(m_main, show_command);
	ShowWindow(m_screen, show_command);
	create_notify();
	RegisterHot();
	InstallKeyboardHook();

	MSG msg{};
	BOOL result = TRUE;

	while ((result = GetMessageW(&msg, nullptr, 0, 0)) != 0)
	{
		if (result == -1)
			return EXIT_FAILURE;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);


	}
	Shell_NotifyIcon(NIM_DELETE, &nid);
	UninstallKeyboardHook();
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
	keyRect = { 0,screenHeight - 250,200,screenHeight};
	hookDisplay = true;
	HWND window = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT,screen_class_name.c_str(), L"screen", WS_OVERLAPPED | WS_POPUP, 0, 0, screenWidth, screenHeight, nullptr, nullptr, m_instance, this);
	SetLayeredWindowAttributes(window, 0, 128, LWA_ALPHA);
	SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_screen = window;
}

void app::InstallKeyboardHook() 
{
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL,&app::KeyboardProc, NULL, 0);
	timerSet = false;
}

void app::UninstallKeyboardHook() {
	if (hHook != NULL) {
		UnhookWindowsHookEx(hHook);
		hHook = NULL;
	}
}

LRESULT app::HandleKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0) 
	{
		KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
		std::wstring keyInfo;

		switch (wParam) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (kbdStruct->vkCode == VK_RCONTROL || kbdStruct->vkCode == VK_LCONTROL)
				keyInfo = L"Ctrl+";
			else if (kbdStruct->vkCode == VK_LSHIFT || kbdStruct->vkCode == VK_RSHIFT)
				keyInfo = L"Shift+";
			else if (kbdStruct->vkCode == VK_LMENU || kbdStruct->vkCode == VK_RMENU)
				keyInfo = L"Alt+";
			else if (kbdStruct->vkCode == VK_CAPITAL)
				keyInfo = L"CAPS+";
			else
				keyInfo = std::wstring(1, char(kbdStruct->vkCode));
			break;
		}
		int length = keyInfo.length();
		if (length > 0)
		{
			keyQueue.push_front(keyInfo);
			if (!timerSet)
			{
				SetTimer(m_screen, ID_TIMER_KEY, 5000, NULL);
				timerSet = true;
				timerID = ID_TIMER_KEY;
			}
			/*else
			{
				timerID++;
				SetTimer(m_screen, timerID, 5000, NULL);
			}*/
			InvalidateRect(m_screen, nullptr, TRUE); 
		}
	}


	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
LRESULT CALLBACK app::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (s_appInstance)
	{
		return s_appInstance->HandleKeyboardProc(nCode, wParam, lParam);
	}
	else
	{
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
}