#include "Window.h"
#include "Utility.h"

HWND Window::s_windowHandle;

unsigned int Window::s_clientWidth;
unsigned int Window::s_clientHeight;

bool		Window::s_mouseCapture;
InputDesc*	Window::s_input;

Window::Window()
{
	//Nop
}
Window::~Window()
{
	SAFE_DELETE(s_input);
}

void Window::checkMessages()
{
	MSG msg = {0};
	if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.message == WM_QUIT)
			m_active = false;
	}
}

bool Window::isActive()
{
	return m_active;
}

void Window::setMouseCapture(bool p_capture)
{
	if(p_capture)
		captureMouse();
	else
		releaseMouse();

	s_mouseCapture = p_capture;
}

unsigned int Window::getClientWidth()
{
	return s_clientWidth;
}
unsigned int Window::getClientHeight()
{
	return s_clientHeight;
}

InputDesc* Window::getUserInput()
{
	return s_input;
}

HWND Window::getWindowHandle()
{
	return s_windowHandle;
}

HRESULT Window::init(HINSTANCE p_hInstance, int p_cmdShow)
{
	HRESULT hr = S_OK;
	hr = createWindow(p_hInstance, p_cmdShow);
	createInputDesc();

	return hr;
}

HRESULT Window::createWindow(HINSTANCE p_hInstance, int p_cmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	 = WindowProc;
	wc.hInstance	 = p_hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	s_clientWidth = 1280;
	s_clientHeight = 1024;

	unsigned int windowPosX = 0;
	unsigned int windowPosY = 0;
	DWORD		 dwExStyle = 0;
	HWND		 hWndParent = 0;
	HMENU		 hMenu = 0;
	LPVOID		 lParam = 0;

	RECT windowRect = {0, 0, s_clientWidth, s_clientHeight};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	s_windowHandle = CreateWindowEx(
		dwExStyle,
		L"WindowClass",
		L"Robbie",
		WS_OVERLAPPEDWINDOW,
		windowPosX,
		windowPosY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		hWndParent,
		hMenu,
		p_hInstance,
		lParam);

	HRESULT hr = S_OK;
	if(!s_windowHandle)
	{
		hr = E_FAIL;
		MessageBox(NULL, L"Window creation failed!", L"Window::init", MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		ShowWindow(s_windowHandle, p_cmdShow);
		m_active = true;
	}
	return hr;
}

void Window::createInputDesc()
{
	s_input = new InputDesc();
	ZeroMemory(s_input, sizeof(*s_input));
}

void Window::captureMouse()
{
	SetCursorPos(s_clientWidth/2, s_clientHeight/2);
	SetCapture(s_windowHandle);
	ShowCursor(false);
}
void Window::releaseMouse()
{
	ReleaseCapture();
	ShowCursor(true);
}

void Window::mouseMove(LPARAM p_lParam)
{
	//Find the upper left corner of the window's client area in screen coordinates
	POINT point;
	point.x = 0;
	point.y = 0;
	MapWindowPoints(s_windowHandle, NULL, &point, 1);
	
	//Get current mouse position
	s_input->mouseX = GET_X_LPARAM(p_lParam);
	s_input->mouseY = GET_Y_LPARAM(p_lParam);

	//Calculate relative mouse movement
	s_input->mouseDeltaX = s_input->mouseX+point.x - s_clientWidth/2;
	s_input->mouseDeltaY = s_input->mouseY+point.y - s_clientHeight/2;

	if(s_mouseCapture)
		SetCursorPos(s_clientWidth/2, s_clientHeight/2);
}

void Window::mouseWheel(WPARAM p_wParam)
{
	s_input->mouseWheelDelta = GET_WHEEL_DELTA_WPARAM(p_wParam)/WHEEL_DELTA;
}

void Window::keyDown(WPARAM p_wParam)
{
	if(!s_input->keys[p_wParam].down)
		s_input->keys[p_wParam].pressed = true;
	else
		s_input->keys[p_wParam].pressed = false;

	s_input->keys[p_wParam].down = true;
}

void Window::keyUp(WPARAM p_wParam)
{
	s_input->keys[p_wParam].down	= false;
	s_input->keys[p_wParam].pressed = false;
}

LRESULT CALLBACK Window::WindowProc(HWND p_hWnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
{

	switch(p_msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_KEYDOWN:
		if(p_wParam == VK_ESCAPE)
			DestroyWindow(p_hWnd);
		keyDown(p_wParam);
		return 0;
		break;
	case WM_KEYUP:
		keyUp(p_wParam);
		return 0;
		break;
	case WM_MOUSEMOVE:
		mouseMove(p_lParam);
		return 0;
		break;
	case WM_MOUSEWHEEL:
		mouseWheel(p_wParam);
		return 0;
		break;
	}

	return DefWindowProc( p_hWnd, p_msg, p_wParam, p_lParam);
}