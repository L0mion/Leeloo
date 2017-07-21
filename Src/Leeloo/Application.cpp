#include "Application.h"

#include "Camera.h"
#include "Raytracer.h"
#include "Window.h"
#include "Utility.h"

#include <iostream>

Application::Application(void)
{
	m_camera = nullptr;
	m_raytracer = nullptr;
	m_window = nullptr;
}

Application::~Application(void)
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_raytracer);
	SAFE_DELETE(m_window);
}

void Application::run()
{
	LARGE_INTEGER freq, old, current;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&old);

	float dt = 0.0f;
	float time = 0.0f;
	while(m_window->isActive())
	{
		QueryPerformanceCounter(&current);
		dt = float (current.QuadPart - old.QuadPart) / float(freq.QuadPart);

		m_window->checkMessages();
		handleInput(dt);

		m_raytracer->render(m_camera, dt);

		old.QuadPart = current.QuadPart;

		time += dt;
		if(time >= 300)
			break;
	}
}

HRESULT Application::init(HINSTANCE p_hInstance, int p_cmdShow)
{
	HRESULT hr = S_OK;

	hr = initWindow(p_hInstance, p_cmdShow);
	if(SUCCEEDED(hr))
	{
		hr = initRaytracer(m_window->getWindowHandle(),
		m_window->getClientWidth(), m_window->getClientHeight());
	}
	if(SUCCEEDED(hr))
		initCamera();

	return hr;
}

void Application::handleInput(float p_dt)
{
	InputDesc* input = m_window->getUserInput();

	float distance = 10.0f * p_dt; 

	if(input->keys[KeyCodes::VK_W].down)
		m_camera->walk(distance);
	if(input->keys[KeyCodes::VK_A].down)
		m_camera->strafe(-distance);
	if(input->keys[KeyCodes::VK_S].down)
		m_camera->walk(-distance);
	if(input->keys[KeyCodes::VK_D].down)
		m_camera->strafe(distance);
	if(input->keys[VK_SPACE].down)
		m_camera->verticalWalk(distance);
	if(input->keys[VK_CONTROL].down)
		m_camera->verticalWalk(-distance);

	if(input->keys[VK_F1].pressed)
		printCameraInfo();

	float mouseSens = 0.005f;
	m_camera->yaw(input->mouseDeltaX*mouseSens);
	m_camera->pitch(input->mouseDeltaY*mouseSens);

	m_camera->updateView();

	input->clearKeyPresses();
}

void Application::printCameraInfo() 
{
	DirectX::XMFLOAT3 pos = m_camera->getPosition();
	DirectX::XMFLOAT3 right = m_camera->getRight();
	DirectX::XMFLOAT3 up = m_camera->getUp();
	DirectX::XMFLOAT3 look = m_camera->getLook();
	std::cout << "Camera pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
	std::cout << "Camera right: " << right.x << ", " << right.y << ", " << right.z << std::endl;
	std::cout << "Camera up: " << up.x << ", " << up.y << ", " << up.z << std::endl;
	std::cout << "Camera look: " << look.x << ", " << look.y << ", " << look.z << std::endl;
}

HRESULT Application::initWindow(HINSTANCE p_hInstance, int p_cmdShow)
{
	HRESULT hr = S_OK;
	m_window = new Window();
	hr = m_window->init(p_hInstance, p_cmdShow);
	if(SUCCEEDED(hr))
		m_window->setMouseCapture(true);
	return hr;
}
HRESULT Application::initRaytracer(HWND p_windowHandle,
								   unsigned int p_clientWidth,
								   unsigned int p_clientHeight)
{
	HRESULT hr = S_OK;
	m_raytracer = new Raytracer();
	hr = m_raytracer->init(p_windowHandle, p_clientWidth, p_clientHeight);
	return hr;
}

void Application::initCamera()
{
	float aspectRatio = static_cast<float>(m_window->getClientWidth()) /
		static_cast<float>(m_window->getClientHeight());
	
	m_camera = new Camera(aspectRatio, DirectX::XM_PIDIV4, 500.0f, 1.0f);
	m_camera->updateView();
	m_camera->updateProj();
}