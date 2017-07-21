#ifndef LEELOO_RAYTRACER_H
#define LEELOO_RAYTRACER_H

#include <vector>
#include <string>

#include <Windows.h>
#include <d3d11.h>

#include "Triangle.h"
#include "Material.h"

class Camera;
class ManagementD3D;
class ManagementShader;
class ManagementCB;
class ManagementSS;
class ManagementLight;
class ManagementTex;
class D3DResource;
class ManagementTimer;

class Raytracer
{
public:
	Raytracer(void);
	virtual ~Raytracer(void);

	void render(Camera* p_camera, float p_dt);

	HRESULT init(HWND p_windowHandle, unsigned int p_clientWidth,
		unsigned int clientHeight);
private:

	void primaryRayStage();
	void prepPrimaryRayStage();
	void cleanPrimaryRayStage();

	void intersectionStage();
	void prepIntersectionStage();
	void cleanIntersectionStage();

	void colorStage();
	void prepColorStage();
	void cleanColorStage();

	HRESULT initManagementD3D(HWND p_windowHandle, unsigned int p_clientWidth,
		unsigned int p_clientHeight);
	HRESULT initManagementShader(ID3D11Device* p_device);
	HRESULT initManagementCB(ID3D11Device* p_device, unsigned int p_clientWidth,
		unsigned int p_clientHeight);
	HRESULT initManagementSS(ID3D11Device* p_device);
	HRESULT initManagementLight(ID3D11Device* p_device);

	HRESULT initAccumulation(ID3D11Device* p_device, unsigned int p_clientWidth, unsigned int p_clientHeight);
	HRESULT initIntersections(ID3D11Device* p_device, unsigned int p_clientWidth, unsigned int p_clientHeight);
	HRESULT initRays(ID3D11Device* p_device, unsigned int p_clientWidth, unsigned int p_clientHeight);

	HRESULT initModels(ID3D11Device* p_device, ID3D11DeviceContext* p_context);

	HRESULT initTriangles(ID3D11Device* p_device, std::vector<Triangle> p_triangles);
	HRESULT initSpheres(ID3D11Device* p_device, unsigned int p_mtlIndex);
	HRESULT initMaterials(ID3D11Device* p_device, std::vector<Material> p_materials);
	HRESULT initManagementTex(ID3D11Device* p_device,
		ID3D11DeviceContext* p_context, std::vector<std::wstring> p_texFilenames);
	HRESULT initManagementTimer(ID3D11Device* p_device);

	unsigned int m_clientWidth;
	unsigned int m_clientHeight;
	unsigned int m_threadCountX;
	unsigned int m_threadCountY;
	unsigned int m_blockSizeX;
	unsigned int m_blockSizeY;
	unsigned int m_traceDepth;
	std::string m_modelName;

	ManagementD3D*		m_managementD3D;
	ManagementShader*	m_managementShader;
	ManagementCB*		m_managementCB;
	ManagementSS*		m_managementSS;
	ManagementLight*	m_managementLight;
	ManagementTex*		m_managementTex;

	D3DResource* m_triangles;
	D3DResource* m_spheres;
	D3DResource* m_materials;

	D3DResource* m_accumulation;
	D3DResource* m_intersections;
	D3DResource* m_rays;

	ManagementTimer* m_managementTimer;
};

#endif //LEELOO_RAYTRACER_H