#define NOMINMAX
#include <windows.h>
#include "app.h"
int WINAPI wWinMain(HINSTANCE instance,
	HINSTANCE /*prevInstance*/,
	LPWSTR /*command_line*/,
	int show_command)
{
	app app{ instance };
	return app.run(show_command);

}