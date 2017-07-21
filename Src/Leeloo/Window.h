#ifndef LEELOO_WINDOW_H
#define LEELOO_WINDOW_H

#include <Windows.h>
#include <windowsx.h>

#include "KeyCodes.h"

struct KeyState
{
	bool down;
	bool pressed;

	KeyState()
	{
		down = false;
		pressed = false;
	}
};

struct InputDesc
{
	KeyState keys[KeyCodes::NUM_KEYS];
	int mouseX;
	int mouseY;
	int mouseDeltaX;
	int mouseDeltaY;
	int mouseWheelDelta;

	void clearKeyPresses()
	{
		for(unsigned int i=0; i<KeyCodes::NUM_KEYS; i++)
			keys[i].pressed = false;
	}
};

class Window
{
public:
	Window();
	~Window();

	void checkMessages();
	bool isActive();

	void setMouseCapture(bool p_capture);

	unsigned int getClientWidth();
	unsigned int getClientHeight();

	InputDesc* getUserInput();

	HWND getWindowHandle();

	HRESULT init(HINSTANCE p_hInstance, int p_cmdShow);
private:

	HRESULT createWindow(HINSTANCE p_hInstance, int p_cmdShow);
	void createInputDesc();

	void captureMouse();
	void releaseMouse();

	static void mouseMove(LPARAM p_lParam);
	static void mouseWheel(WPARAM p_wParam);
	static void keyDown(WPARAM p_wParam);
	static void keyUp(WPARAM p_wParam);

	static LRESULT CALLBACK WindowProc( HWND p_hWnd, UINT p_msg,
		WPARAM p_wParam, LPARAM p_lParam);

	bool m_active;

	static HWND s_windowHandle;

	static unsigned int s_clientWidth;
	static unsigned int s_clientHeight;
	
	static bool		  s_mouseCapture;	
	static InputDesc* s_input;
};

#endif //LEELOO_WINDOW_H