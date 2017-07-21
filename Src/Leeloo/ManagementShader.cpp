#include "ManagementShader.h"
#include "ShaderSet.h"
#include "Utility.h"

ManagementShader::ManagementShader()
{
	for(unsigned int i=0; i<ShaderIds::ShaderIds_COUNT; i++)
		m_shaders[i] = nullptr;
}
ManagementShader::~ManagementShader()
{
	for(unsigned int i=0; i<ShaderIds::ShaderIds_COUNT; i++)
		SAFE_DELETE(m_shaders[i]);
}

void ManagementShader::setShader(ID3D11DeviceContext* p_context, ShaderIds::Id p_shaderId)
{
	m_shaders[p_shaderId]->set(p_context);
}

void ManagementShader::unsetShader(ID3D11DeviceContext* p_context)
{
	p_context->VSSetShader(nullptr, nullptr, 0);
	p_context->PSSetShader(nullptr, nullptr, 0);
	p_context->CSSetShader(nullptr, nullptr, 0);
	p_context->IASetInputLayout(nullptr);
}

HRESULT ManagementShader::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	hr = initShaders(p_device);
	if(SUCCEEDED(hr))
		hr = initInputLayouts(p_device);

	return hr;
}
HRESULT ManagementShader::initShaders(ID3D11Device* p_device)
{
	for(unsigned int i=0; i<ShaderIds::ShaderIds_COUNT; i++)
		m_shaders[i] = new ShaderSet();

	HRESULT hr = S_OK;

	hr = initComputeShader(p_device, 
		L"CsPrimaryRayStage.cso", 
		m_shaders[ShaderIds::ShaderIds_PRIMARY_RAY_STAGE]->getCSPtr(), 
		m_shaders[ShaderIds::ShaderIds_PRIMARY_RAY_STAGE]->getCSBlobPtr());
	if(SUCCEEDED(hr))
	{
		hr = initComputeShader(p_device, 
		L"CsIntersectionStage.cso", 
		m_shaders[ShaderIds::ShaderIds_INTERSECTION_STAGE]->getCSPtr(), 
		m_shaders[ShaderIds::ShaderIds_INTERSECTION_STAGE]->getCSBlobPtr());
	}
	if(SUCCEEDED(hr))
	{
		hr = initComputeShader(p_device, 
		L"CsColorStage.cso", 
		m_shaders[ShaderIds::ShaderIds_COLOR_STAGE]->getCSPtr(), 
		m_shaders[ShaderIds::ShaderIds_COLOR_STAGE]->getCSBlobPtr());
	}

	return hr;
}
HRESULT ManagementShader::initVertexShader( ID3D11Device* p_device, std::wstring p_filePath, ID3D11VertexShader** io_vs, ID3DBlob** io_blob )
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(p_filePath.c_str(), io_blob);
	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetShader::initVertexShader() | D3DReadFileToBlob() | Failed", p_filePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	else
	{
		hr = p_device->CreateVertexShader((*io_blob)->GetBufferPointer(), (*io_blob)->GetBufferSize(), NULL, io_vs);
		if(FAILED(hr))
			MessageBox(NULL, L"ManagementShader::initVertexShader() | device->CreateVertexShader() | Failed", p_filePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	}

	return hr;
}
HRESULT ManagementShader::initPixelShader(ID3D11Device* p_device, std::wstring p_filePath, ID3D11PixelShader** io_ps, ID3DBlob** io_blob)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(p_filePath.c_str(), io_blob);
	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetShader::initPixelShader() | D3DReadFileToBlob() | Failed", p_filePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	else
	{
		hr = p_device->CreatePixelShader((*io_blob)->GetBufferPointer(), (*io_blob)->GetBufferSize(), NULL, io_ps);
		if(FAILED(hr))
			MessageBox(NULL, L"ManagementShader::initPixelShader() | device->CreatePixelShader() | Failed", p_filePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	}

	return hr;
}
HRESULT ManagementShader::initComputeShader(ID3D11Device* p_device, std::wstring p_filePath, ID3D11ComputeShader** io_cs, ID3DBlob** io_blob)
{
	HRESULT hr = S_OK;

	hr = D3DReadFileToBlob(p_filePath.c_str(), io_blob);
	if(FAILED(hr))
	{
		MessageBox(NULL, L"ManagmenetShader::initComputeShader() | D3DReadFileToBlob() | Failed",
					p_filePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		hr = p_device->CreateComputeShader((*io_blob)->GetBufferPointer(), (*io_blob)->GetBufferSize(), NULL, io_cs);
		if(FAILED(hr))
		{
			MessageBox(NULL, L"ManagementShader::initComputeShader() | device->CreatePixelShader() | Failed",
						p_filePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
		}
	}
	return hr;
}

HRESULT ManagementShader::initInputLayouts(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	return hr;
}