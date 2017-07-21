#include "D3DResource.h"
#include "Utility.h"

D3DResource::D3DResource()
{
	m_buffer = nullptr;
	m_srv	 = nullptr;
	m_uav	 = nullptr;
}
D3DResource::~D3DResource()
{
	SAFE_RELEASE(m_buffer);
	SAFE_RELEASE(m_srv);
	SAFE_RELEASE(m_uav);
}

void D3DResource::csSetSRV(ID3D11DeviceContext* p_context, unsigned int p_register)
{
	p_context->CSSetShaderResources(p_register, 1, &m_srv);
}

void D3DResource::csSetUAV(ID3D11DeviceContext* p_context, unsigned int p_register)
{
	p_context->CSSetUnorderedAccessViews(p_register, 1, &m_uav, nullptr);
}

unsigned int D3DResource::getElementCnt()
{
	unsigned int elementCnt = 0;
	if(m_buffer)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		m_buffer->GetDesc(&bufferDesc);
		elementCnt = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
	}
	return elementCnt;
}

ID3D11ShaderResourceView* D3DResource::getSrv() const
{
	return m_srv;
}
ID3D11ShaderResourceView** D3DResource::getSrvPtr()
{
	return &m_srv;
}
ID3D11UnorderedAccessView* D3DResource::getUav() const
{
	return m_uav;
}
ID3D11UnorderedAccessView** D3DResource::getUavPtr()
{
	return &m_uav;
}

HRESULT D3DResource::init(ID3D11Device* p_device, unsigned int p_elementSize, unsigned int p_elementCnt, void* p_initData)
{
	HRESULT hr = S_OK;

	hr = createStructuredBuffer(p_device, p_elementSize, p_elementCnt, p_initData);
	if(SUCCEEDED(hr))
		hr = createShaderResourceView(p_device);
	if(SUCCEEDED(hr))
		hr = createUnorderedAccessView(p_device);

	return hr;
}

HRESULT D3DResource::createStructuredBuffer(ID3D11Device* p_device, unsigned p_elementSize, unsigned p_elementCnt, void* p_initData)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.BindFlags			= D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.ByteWidth			= p_elementSize * p_elementCnt;
	bufferDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride	= p_elementSize;

	if(p_initData)
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = p_initData;
		hr = p_device->CreateBuffer(&bufferDesc, &initData, &m_buffer);
	}
	else
		hr = p_device->CreateBuffer(&bufferDesc, nullptr, &m_buffer);

	if(FAILED(hr))
		MessageBox(0, L"D3DResource::createStructuredBuffer() Failed", L"D3DResource.h", MB_OK | MB_ICONEXCLAMATION);
	
	return hr;
}
HRESULT D3DResource::createShaderResourceView(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	m_buffer->GetDesc(&bufferDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement	= 0;
	srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
	srvDesc.BufferEx.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

	hr = p_device->CreateShaderResourceView(m_buffer, &srvDesc, &m_srv);
	if(FAILED(hr))
		MessageBox(0, L"D3DResource::createShaderResourceView() Failed", L"D3DResource.h", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}
HRESULT D3DResource::createUnorderedAccessView(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	m_buffer->GetDesc(&bufferDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));

	uavDesc.ViewDimension		= D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Format				= DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

	hr = p_device->CreateUnorderedAccessView(m_buffer, &uavDesc, &m_uav);
	if(FAILED(hr))
		MessageBox(0, L"D3DResource::createUnorderedAccessView() Failed", L"D3DResource.h", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}