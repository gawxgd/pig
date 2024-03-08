#pragma once

#include <windows.h>
#include <string>
#include <ctime>
#define pocisk_count 5
class app
{
private:
	bool register_class();
	static std::wstring const s_class_name;
	static LRESULT CALLBACK window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	HWND create_window(DWORD stykle);
	//static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam);
	//void create_tank();
	//void UpdateWindowTitle(HWND window)
	//{
	//	DWORD elapsedTime = GetTickCount() - startTime;

	//	// Convert milliseconds to seconds
	//	int seconds = elapsedTime / 1000;
	//	std::string s = std::to_string(score);

	//	// Format the time as a string
	//	std::string title = s + "Elapsed Time: " + std::to_string(seconds) + " seconds";

	//	// Set the window title
	//	SetWindowTextA(m_main, title.c_str());
	//}

	/*void move_tank(int x, int y);
	void create_coin();
	void czolg_robi_strzal(int orient);
	int orient;
	HINSTANCE m_instance;
	HWND m_main;
	POINT m_screen_size;
	HWND m_tank;
	HBRUSH tank_color;
	HWND m_coin;
	int score;
	DWORD startTime;
	HWND m_pocisk[pocisk_count];*/
	HWND m_main;
	HINSTANCE m_instance;
	int sizeCount;
	int change;
	void RegisterHot()
	{
		// Register Ctrl+Alt+H as the hotkey
		if (RegisterHotKey(m_main, 2, MOD_CONTROL | MOD_ALT, 'C'))
		{
			// Hotkey registered successfully
		}
		else
		{
			//ShowErrorMessageBox()
		}
		if (RegisterHotKey(m_main, 1, 0, VK_F4))
		{
			// Hotkey registered successfully
		}
		else
		{
			//ShowErrorMessageBox()
		}
	}
	void ShowErrorMessageBox(DWORD errorCode)
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
		MessageBox(
			NULL,
			errorMsgBuffer ? static_cast<LPCWSTR>(errorMsgBuffer) : L"Unknown error",
			L"Error",
			MB_ICONERROR | MB_OK
		);

		// Free the buffer allocated by FormatMessage
		LocalFree(errorMsgBuffer);
	}


public:
	app(HINSTANCE instance);
	int run(int show_command);
	~app() {}
};