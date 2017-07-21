#include "ManagementCB.h"
#include "Utility.h"

ManagementCB::ManagementCB()
{
	for(unsigned int i=0; i<CBIds_COUNT; i++)
		m_buffers[i] = nullptr;
}
ManagementCB::~ManagementCB()
{
	for(unsigned int i=0; i<CBIds_COUNT; i++)
		SAFE_RELEASE(m_buffers[i]);
}

void ManagementCB::vsSetCB(ID3D11DeviceContext* p_context, CBIds p_cbId)
{
	p_context->VSSetConstantBuffers(p_cbId, 1, &m_buffers[p_cbId]);
}
void ManagementCB::psSetCB(ID3D11DeviceContext* p_context, CBIds p_cbId)
{
	p_context->PSSetConstantBuffers(p_cbId, 1, &m_buffers[p_cbId]);
}
void ManagementCB::csSetCB(ID3D11DeviceContext* p_context, CBIds p_cbId)
{
	p_context->CSSetConstantBuffers(p_cbId, 1, &m_buffers[p_cbId]);
}

void ManagementCB::updateCBFrame(ID3D11DeviceContext* p_context, CBFrame p_cb)
{
	p_context->UpdateSubresource(m_buffers[CBIds_FRAME], 0, 0, &p_cb, 0, 0);
}

void ManagementCB::updateCBWindowResize(ID3D11DeviceContext* p_context, CBWindowResize p_cb)
{
	p_context->UpdateSubresource(m_buffers[CBIds_WINDOW_RESIZE], 0, 0, &p_cb, 0, 0);
}

void ManagementCB::updateCBObject(ID3D11DeviceContext* p_context, CBObject p_cb)
{
	p_context->UpdateSubresource(m_buffers[CBIds_OBJECT], 0, 0, &p_cb, 0, 0);
}

HRESULT ManagementCB::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	hr = initConstantBuffer(p_device, &m_buffers[CBIds_FRAME], sizeof(CBFrame));
	if(SUCCEEDED(hr))
		hr = initConstantBuffer(p_device, &m_buffers[CBIds_WINDOW_RESIZE], sizeof(CBWindowResize));
	if(SUCCEEDED(hr))
		hr = initConstantBuffer(p_device, &m_buffers[CBIds_OBJECT], sizeof(CBObject));

	return hr;
}

HRESULT ManagementCB::initConstantBuffer(ID3D11Device* p_device, ID3D11Buffer** io_cb, unsigned int p_size)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= p_size;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;
	
	hr = p_device->CreateBuffer(&bufferDesc, NULL, io_cb);
	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetCB::initConstantBuffer() | CreateBuffer() | Failed", L"ConstantBuffer fail", MB_OK | MB_ICONEXCLAMATION); 
	
	return hr;
}