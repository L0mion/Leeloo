#include "Raytracer.h"

#include "IntersectDesc.h"
#include "RayDesc.h"
#include "SphereDesc.h"
#include "D3DResource.h"
#include "ManagementD3D.h"
#include "Camera.h"
#include "ManagementShader.h"
#include "ManagementCB.h"
#include "ManagementSS.h"
#include "ManagementLight.h"
#include "ManagementTex.h"
#include "ManagementTimer.h"
#include "ObjLoader.h"
#include "Utility.h"

#include <sstream>

Raytracer::Raytracer(void)
{
	m_managementD3D		= nullptr;
	m_managementShader	= nullptr;
	m_managementCB		= nullptr;
	m_managementSS		= nullptr;
	m_managementLight	= nullptr;
	m_managementTex		= nullptr;

	m_triangles			= nullptr;
	m_spheres			= nullptr;
	m_materials			= nullptr;

	m_accumulation		= nullptr;
	m_intersections		= nullptr;
	m_rays				= nullptr;

	m_managementTimer	= nullptr;
}

Raytracer::~Raytracer(void)
{
	SAFE_DELETE(m_managementD3D);
	SAFE_DELETE(m_managementShader);
	SAFE_DELETE(m_managementCB);
	SAFE_DELETE(m_managementSS);
	SAFE_DELETE(m_managementLight);
	SAFE_DELETE(m_managementTex);

	SAFE_DELETE(m_triangles);
	SAFE_DELETE(m_spheres);
	SAFE_DELETE(m_materials);

	SAFE_DELETE(m_accumulation);
	SAFE_DELETE(m_intersections);
	SAFE_DELETE(m_rays);

	m_managementTimer->printToFile();
	SAFE_DELETE(m_managementTimer);
}



void Raytracer::render(Camera* p_camera, float p_dt)
{
	ID3D11DeviceContext* context = m_managementD3D->getDeviceContext();	
	
	m_managementLight->moveLights(context, p_dt);

	CBFrame cBuffer;
	cBuffer.numLights		= m_managementLight->getNumLights();
	cBuffer.cameraPosition	= p_camera->getPosition();
	cBuffer.projMatrix		= p_camera->getProjection();
	cBuffer.projMatrixInv	= p_camera->getProjectionInv();
	cBuffer.viewMatrix		= p_camera->getView();
	cBuffer.viewMatrixInv	= p_camera->getViewInv();
	
	m_managementCB->csSetCB(context, ManagementCB::CBIds::CBIds_FRAME);
	m_managementCB->updateCBFrame(context, cBuffer);

	primaryRayStage();
	
	for(unsigned int i=0; i<m_traceDepth; i++)
	{
		intersectionStage();
		colorStage();
	}

	m_managementD3D->present();
	m_managementTimer->finnishFrame();
}



HRESULT Raytracer::init(HWND p_windowHandle, unsigned int p_clientWidth,
						unsigned int p_clientHeight)
{
	HRESULT hr = S_OK;

	m_blockSizeX = 16;
	m_blockSizeY = 16;
	m_traceDepth = 3;
	m_modelName = "box24.obj";

	m_clientWidth = p_clientWidth;
	m_clientHeight = p_clientHeight;

	m_threadCountX = p_clientWidth/m_blockSizeX;
	if(p_clientWidth % m_blockSizeX != 0)
		m_threadCountX += 1;
	m_threadCountY = p_clientHeight/m_blockSizeY;
	if(p_clientHeight % m_blockSizeY != 0)
		m_threadCountY += 1;

	hr = initManagementD3D(p_windowHandle, p_clientWidth, p_clientHeight);
	if(SUCCEEDED(hr))
		hr = initManagementShader(m_managementD3D->getDevice());
	if(SUCCEEDED(hr))
		hr = initManagementCB(m_managementD3D->getDevice(), p_clientWidth, p_clientHeight);
	if(SUCCEEDED(hr))
		hr = initManagementSS(m_managementD3D->getDevice());
	if(SUCCEEDED(hr))
		hr = initManagementLight(m_managementD3D->getDevice());
	if(SUCCEEDED(hr))
		hr = initAccumulation(m_managementD3D->getDevice(), p_clientWidth, p_clientHeight);
	if(SUCCEEDED(hr))
		hr = initIntersections(m_managementD3D->getDevice(), p_clientWidth, p_clientHeight);
	if(SUCCEEDED(hr))
		hr = initRays(m_managementD3D->getDevice(), p_clientWidth, p_clientHeight);
	if(SUCCEEDED(hr))
		hr = initModels(m_managementD3D->getDevice(), m_managementD3D->getDeviceContext());
	if(SUCCEEDED(hr))
		hr = initManagementTimer(m_managementD3D->getDevice());
	return hr;
}



void Raytracer::primaryRayStage()
{
	prepPrimaryRayStage();
	ID3D11DeviceContext* context = m_managementD3D->getDeviceContext();
	m_managementTimer->start(TimerIds::TimerIds_PRIMARY_RAY_STAGE, context);
	context->Dispatch(m_threadCountX, m_threadCountY, 1);
	m_managementTimer->stop(TimerIds::TimerIds_PRIMARY_RAY_STAGE, context);
	cleanPrimaryRayStage();
}

void Raytracer::prepPrimaryRayStage()
{
	ID3D11DeviceContext* context = m_managementD3D->getDeviceContext();
	m_managementShader->setShader(context, ShaderIds::ShaderIds_PRIMARY_RAY_STAGE);
	m_accumulation->csSetUAV(context, 1);
	m_rays->csSetUAV(context, 3);
}

void Raytracer::cleanPrimaryRayStage()
{
	ID3D11DeviceContext* context = m_managementD3D->getDeviceContext();
	ID3D11UnorderedAccessView* uav = nullptr;
	context->CSSetUnorderedAccessViews(1, 1, &uav, nullptr);
	context->CSSetUnorderedAccessViews(3, 1, &uav, nullptr);
	m_managementShader->unsetShader(context);
}



void Raytracer::intersectionStage()
{
	prepIntersectionStage();
	ID3D11DeviceContext* context = m_managementD3D->getDeviceContext();
	m_managementTimer->start(TimerIds::TimerIds_INTERSECTION_STAGE, context);
	context->Dispatch(m_threadCountX, m_threadCountY, 1);
	m_managementTimer->stop(TimerIds::TimerIds_INTERSECTION_STAGE, context);
	cleanIntersectionStage();
}

void Raytracer::prepIntersectionStage()
{
	ID3D11DeviceContext* context = m_managementD3D->getDeviceContext();

	m_managementShader->setShader(context, ShaderIds::ShaderIds_INTERSECTION_STAGE);
	m_intersections->csSetUAV(context, 2);
	m_rays->csSetUAV(context, 3);
	m_triangles->csSetSRV(context, 0);
	m_spheres->csSetSRV(context, 3);

	m_managementCB->csSetCB(context, ManagementCB::CBIds_OBJECT);
	CBObject cBuffer;
	cBuffer.numTriangles = m_triangles->getElementCnt();
	cBuffer.numSpheres = m_spheres->getElementCnt();
	m_managementCB->updateCBObject(context, cBuffer);
}

void Raytracer::cleanIntersectionStage()
{
	ID3D11DeviceContext* context = m_managementD3D->getDeviceContext();
	ID3D11UnorderedAccessView* uav = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
	context->CSSetUnorderedAccessViews(2, 1, &uav, nullptr);
	context->CSSetUnorderedAccessViews(3, 1, &uav, nullptr);
	context->CSSetShaderResources(0, 1, &srv);
	m_managementShader->unsetShader(context);
}



void Raytracer::colorStage()
{
	prepColorStage();
	ID3D11DeviceContext* context = m_managementD3D->getDeviceContext();
	m_managementTimer->start(TimerIds::TimerIds_COLOR_STAGE, context);
	context->Dispatch(m_threadCountX, m_threadCountY, 1);
	m_managementTimer->stop(TimerIds::TimerIds_COLOR_STAGE, context);
	cleanColorStage();
}

void Raytracer::prepColorStage()
{
	ID3D11DeviceContext* context = m_managementD3D->getDeviceContext();
	m_managementShader->setShader(context, ShaderIds::ShaderIds_COLOR_STAGE);
	m_managementD3D->setBackBuffer();

	m_accumulation->csSetUAV(context, 1);
	m_intersections->csSetUAV(context, 2);
	m_rays->csSetUAV(context, 3);

	m_triangles->csSetSRV(context, 0);
	m_materials->csSetSRV(context, 1);
	m_managementLight->csSetLightSRV(context, 2);
	m_spheres->csSetSRV(context, 3);
	m_managementTex->csSetTexture(context, 4);

	m_managementSS->csSetSS(context, ManagementSS::SSTypes_DEFAULT, 0);
}

void Raytracer::cleanColorStage()
{
	ID3D11DeviceContext* context = m_managementD3D->getDeviceContext();

	ID3D11UnorderedAccessView* uav = nullptr;	
	context->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);
	context->CSSetUnorderedAccessViews(1, 1, &uav, nullptr);
	context->CSSetUnorderedAccessViews(2, 1, &uav, nullptr);
	context->CSSetUnorderedAccessViews(3, 1, &uav, nullptr);

	ID3D11ShaderResourceView* srv = nullptr;
	context->CSSetShaderResources(0, 1, &srv);
	context->CSSetShaderResources(1, 1, &srv);
	context->CSSetShaderResources(2, 1, &srv);
	context->CSSetShaderResources(3, 1, &srv);
	context->CSSetShaderResources(4, 1, &srv);

	m_managementShader->unsetShader(context);
}



HRESULT Raytracer::initManagementD3D(HWND p_windowHandle,
									 unsigned int p_clientWidth,
									 unsigned int p_clientHeight)
{
	HRESULT hr = S_OK;
	m_managementD3D = new ManagementD3D();
	hr = m_managementD3D->init(p_windowHandle, p_clientWidth, p_clientHeight);
	return hr;
}

HRESULT Raytracer::initManagementShader(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;
	m_managementShader = new ManagementShader();
	m_managementShader->init(p_device);
	return hr;
}

HRESULT Raytracer::initManagementCB(ID3D11Device* p_device, unsigned int p_clientWidth, unsigned int p_clientHeight)
{
	HRESULT hr = S_OK;
	m_managementCB = new ManagementCB();
	hr = m_managementCB->init(p_device);

	if(SUCCEEDED(hr))
	{
		m_managementCB->csSetCB(m_managementD3D->getDeviceContext(), ManagementCB::CBIds::CBIds_WINDOW_RESIZE);
		
		CBWindowResize cb;
		cb.screenWidth = p_clientWidth;
		cb.screenHeight = p_clientHeight;
		m_managementCB->updateCBWindowResize(m_managementD3D->getDeviceContext(), cb);
	}

	return hr;
}

HRESULT Raytracer::initManagementSS(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;
	m_managementSS = new ManagementSS();
	hr = m_managementSS->init(p_device);
	return hr;
}

HRESULT Raytracer::initManagementLight(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;
	m_managementLight = new ManagementLight();
	hr = m_managementLight->init(p_device);
	return hr;
}

HRESULT Raytracer::initAccumulation(ID3D11Device* p_device, unsigned int p_clientWidth, unsigned int p_clientHeight)
{
	HRESULT hr = S_OK;
	unsigned int elementSize = 16;
	unsigned int elementCount = p_clientWidth * p_clientHeight;
	m_accumulation = new D3DResource();
	hr = m_accumulation->init(p_device, elementSize, elementCount, nullptr);
	return hr;
}

HRESULT Raytracer::initIntersections(ID3D11Device* p_device, unsigned int p_clientWidth, unsigned int p_clientHeight)
{
	HRESULT hr = S_OK;
	unsigned int elementSize = sizeof(IntersectDesc);
	unsigned int elementCount = p_clientWidth * p_clientHeight; 
	m_intersections = new D3DResource();
	hr = m_intersections->init(p_device, elementSize, elementCount, nullptr);
	return hr;
}

HRESULT Raytracer::initRays(ID3D11Device* p_device, unsigned int p_clientWidth, unsigned int p_clientHeight)
{
	HRESULT hr = S_OK;
	unsigned int elementSize = sizeof(RayDesc);
	unsigned int elementCount = p_clientWidth*p_clientHeight;
	m_rays = new D3DResource();
	hr = m_rays->init(p_device, elementSize, elementCount, nullptr);
	return hr;
}

HRESULT Raytracer::initModels(ID3D11Device* p_device, ID3D11DeviceContext* p_context)
{
	HRESULT hr = S_OK;

	std::stringstream ss;
	ss << "../Resources/" << m_modelName;
	ObjLoader objLoader;
	objLoader.loadObj(ss.str());

	std::vector<Triangle>	triangles	= objLoader.getLoadedTriangles();
	std::vector<Mtl>		mtls		= objLoader.getLoadedMtls();

	std::vector<Material> materials;
	std::vector<std::wstring> textureNames;
	for(unsigned int i=0; i<mtls.size(); i++)
	{
		materials.push_back(Material(mtls[i].ambient, mtls[i].diffuse, mtls[i].specular));
		textureNames.push_back(mtls[i].diffuseTexName);
	}
	if(materials.size() == 0)
		materials.push_back(Material());

	Material mtl;
	mtl.ambient = DirectX::XMFLOAT4(0.0f, 0.4f, 0.0f, 1.0f);
	mtl.diffuse = DirectX::XMFLOAT4(0.0f, 0.4f, 0.0f, 1.0f);
	mtl.specular = DirectX::XMFLOAT4(0.55f, 0.55f, 0.55f, 1.0f);
	materials.push_back(mtl);

	hr = initTriangles(p_device, triangles);
	if(SUCCEEDED(hr))
		hr = initSpheres(p_device, materials.size()-1);
	if(SUCCEEDED(hr))
		hr = initMaterials(p_device, materials);
	if(SUCCEEDED(hr))
		hr = initManagementTex(p_device, p_context, textureNames);

	return hr;
}

HRESULT Raytracer::initTriangles(ID3D11Device* p_device, std::vector<Triangle> p_triangles)
{
	HRESULT hr = S_OK;
	m_triangles = new D3DResource();
	hr = m_triangles->init(p_device, sizeof(p_triangles[0]), p_triangles.size(), (void*)&p_triangles[0]);
	return hr;
}

HRESULT Raytracer::initSpheres(ID3D11Device* p_device, unsigned int p_mtlIndex)
{
	const int SPHERE_CNT = 1;
	SphereDesc spheres[SPHERE_CNT];
	spheres[0].center = DirectX::XMFLOAT3(8.0f, 0.0f, 8.0f);
	spheres[0].radius = 1.0f;
	spheres[0].materialIndex = p_mtlIndex;

	m_spheres = new D3DResource();
	HRESULT hr = m_spheres->init(p_device, sizeof(spheres[0]), SPHERE_CNT, (void*)spheres);

	return hr;
}

HRESULT Raytracer::initMaterials(ID3D11Device* p_device, std::vector<Material> p_materials)
{
	HRESULT hr = S_OK;
	m_materials = new D3DResource();
	hr = m_materials->init(p_device, sizeof(p_materials[0]), p_materials.size(), (void*)&p_materials[0]);
	return hr;
}

HRESULT Raytracer::initManagementTex(ID3D11Device* p_device,
									 ID3D11DeviceContext* p_context,
									 std::vector<std::wstring> p_texFilenames)
{
	HRESULT hr = S_OK;
	m_managementTex = new ManagementTex();
	m_managementTex->init(p_device, p_context, p_texFilenames);
	return hr;
}

HRESULT Raytracer::initManagementTimer(ID3D11Device* p_device)
{
	std::stringstream ss;
	ss	<< "__Res" << m_clientWidth <<"x"<< m_clientHeight
		<< "__X" << m_blockSizeX
		<< "__Y" << m_blockSizeY
		<< "__TraceDepth" << m_traceDepth
		<< "__Lights" << m_managementLight->getNumLights()
		<< "__obj_" << m_modelName
		<< ".txt";

	HRESULT hr = S_OK;
	m_managementTimer = new ManagementTimer(ss.str());
	m_managementTimer->init(p_device);
	return hr;
}