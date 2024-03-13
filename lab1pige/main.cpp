#define NOMINMAX
#include <windows.h>
#include "app.h"
#include "screenWindow.h"



int WINAPI wWinMain(HINSTANCE instance,
	HINSTANCE /*prevInstance*/,
	LPWSTR /*command_line*/,
	int show_command)
{
	app app_instance{ instance };

	// odpalic wi innym watku

	return app_instance.run(show_command);

}