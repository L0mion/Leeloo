
#include <iostream>
#include <Windows.h>

#if defined( DEBUG ) || defined( _DEBUG )
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


#include "Application.h"

int WINAPI WinMain( HINSTANCE p_hInstance, HINSTANCE p_hPrevInstance, LPSTR p_lpCmdLine, int p_cmdShow )
{
#if defined( DEBUG ) || defined( _DEBUG )
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

	AllocConsole();
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 1040, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Application* app = new Application();
	HRESULT hr = app->init(p_hInstance, p_cmdShow);
	if(SUCCEEDED(hr))
		app->run();

	delete app;

	return 0;
}