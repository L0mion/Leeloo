#ifndef LEELOO_MANAGEMENTD3D_H
#define LEELOO_MANAGEMENTD3D_H

#include <d3d11.h>

class ManagementD3D
{
public:
	ManagementD3D();
	~ManagementD3D();

	IDXGISwapChain*		 getSwapChain();
	ID3D11Device*		 getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void present();

	void setBackBuffer();

	HRESULT init(HWND p_windowHandle, unsigned int p_clientWidth,
		unsigned int p_clientHeight);

private:

	DXGI_SWAP_CHAIN_DESC createSwapChainDesc(HWND p_windowHandle,
		unsigned int p_clientWidth, unsigned int p_clientHeight);

	HRESULT createDeviceAndSwapChain(D3D_FEATURE_LEVEL* io_initiatedFeatureLvl,
		DXGI_SWAP_CHAIN_DESC p_swapChainDesc);

	HRESULT initUAVBackBuffer();

	IDXGISwapChain*				m_swapChain;
	ID3D11Device*				m_device;
	ID3D11DeviceContext*		m_context;
	ID3D11UnorderedAccessView*	m_uavBackBuffer;
};

#endif //LEELOO_MANAGEMENTD3D_H