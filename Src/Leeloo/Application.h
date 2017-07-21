#ifndef LEELOO_APPLICATION_H
#define LEELOO_APPLICATION_H

#include <Windows.h>

class Camera;
class Raytracer;
class Window;

class Application
{
public:
	Application(void);
	~Application(void);

	void run();

	HRESULT init(HINSTANCE p_hInstance, int p_cmdShow);
private:

	void handleInput(float p_dt);
	void printCameraInfo();

	HRESULT initWindow(HINSTANCE p_hInstance, int p_cmdShow);
	HRESULT initRaytracer(HWND p_windowHandle, unsigned int p_clientWidth,
		unsigned int );
	void initCamera();

	Window*		m_window;
	Raytracer*	m_raytracer;
	Camera*		m_camera;
	
};

#endif //LEELOO_APPLICATION_H