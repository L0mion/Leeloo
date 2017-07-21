#include "ManagementD3D.h"
#include "Utility.h"

ManagementD3D::ManagementD3D()
{
	m_swapChain		= nullptr;
	m_device		= nullptr;
	m_context		= nullptr;
	m_uavBackBuffer = nullptr;
}

ManagementD3D::~ManagementD3D()
{
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_context);
	SAFE_RELEASE(m_uavBackBuffer);
}

IDXGISwapChain* ManagementD3D::getSwapChain()
{
	return m_swapChain;
}

ID3D11Device* ManagementD3D::getDevice()
{
	return m_device;
}

ID3D11DeviceContext* ManagementD3D::getDeviceContext()
{
	return m_context;
}

void ManagementD3D::present()
{
	m_swapChain->Present(0, 0);
}

void ManagementD3D::setBackBuffer()
{
	m_context->CSSetUnorderedAccessViews(0, 1, &m_uavBackBuffer, nullptr);
}

HRESULT ManagementD3D::init(HWND p_windowHandle, unsigned int p_clientWidth,
							unsigned int p_clientHeight)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = createSwapChainDesc(p_windowHandle,
		p_clientWidth, p_clientHeight);

	D3D_FEATURE_LEVEL initiatedFeatureLvl;
	HRESULT hr = createDeviceAndSwapChain(&initiatedFeatureLvl, swapChainDesc);
	if(SUCCEEDED(hr))
		hr = initUAVBackBuffer();

	return hr;
}

DXGI_SWAP_CHAIN_DESC ManagementD3D::createSwapChainDesc(
	HWND p_windowHandle, unsigned int p_clientWidth, unsigned int p_clientHeight)
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BufferCount						= 1;
	desc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.Width					= p_clientWidth;
	desc.BufferDesc.Height					= p_clientHeight;
	desc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	desc.OutputWindow						= p_windowHandle;
	desc.BufferDesc.RefreshRate.Numerator	= 60;
	desc.BufferDesc.RefreshRate.Denominator	= 1;
	desc.SampleDesc.Count					= 1;
	desc.SampleDesc.Quality					= 0;
	desc.Windowed							= true;
	desc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	return desc;
}

HRESULT ManagementD3D::createDeviceAndSwapChain(
	D3D_FEATURE_LEVEL* io_initiatedFeatureLvl, DXGI_SWAP_CHAIN_DESC p_swapChainDesc)
{
	UINT featureLvlCnt = 3;
	D3D_FEATURE_LEVEL featureLvls[] = {D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0};

	UINT driverTypeCnt = 2;
	D3D_DRIVER_TYPE driverTypes[] = {D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE};

	UINT index = 0;
	HRESULT hr = E_FAIL;

	UINT deviceFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //DEBUG || _DEBUG

	while(index < driverTypeCnt && hr != S_OK)
	{
		hr = D3D11CreateDeviceAndSwapChain(NULL,
			driverTypes[index],
			NULL,
			deviceFlags,
			featureLvls,
			featureLvlCnt,
			D3D11_SDK_VERSION,
			&p_swapChainDesc,
			&m_swapChain,
			&m_device,
			io_initiatedFeatureLvl,
			&m_context);
		
		index++;
	}

	return hr;
}

HRESULT ManagementD3D::initUAVBackBuffer()
{
	HRESULT hr = S_OK;
	ID3D11Texture2D* texBackBuffer;
	m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&texBackBuffer );
	hr = m_device->CreateUnorderedAccessView( texBackBuffer, nullptr, &m_uavBackBuffer );
	SAFE_RELEASE(texBackBuffer);
	return hr;
}