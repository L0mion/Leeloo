#include "ManagementSS.h"
#include "Utility.h"

ManagementSS::ManagementSS()
{
}
ManagementSS::~ManagementSS()
{
	for( unsigned int i=0; i<SSTypes_COUNT; i++ ) {
		SAFE_RELEASE( m_samplerStates[i] );
	}
	
}

void ManagementSS::csSetSS( ID3D11DeviceContext* p_devcon, SSTypes p_ssType,
	unsigned int p_startSlot )
{
	p_devcon->CSSetSamplers( p_startSlot, 1, &m_samplerStates[p_ssType] );
}

HRESULT ManagementSS::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	hr = initSSDefault( p_device );
	hr = initSSWrap( p_device );

	return hr;
}
HRESULT ManagementSS::initSSDefault(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU		= D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV		= D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW		= D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD			= 0;
	sampDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	hr = p_device->CreateSamplerState(&sampDesc, &m_samplerStates[SSTypes_DEFAULT]);

	if(FAILED(hr))
	{
		MessageBox( NULL,
			L"ManagementSS::initSSDefault() | device->CreateSamplerState() | Failed",
			L"ssDefault", MB_OK | MB_ICONEXCLAMATION );
	} 
	return hr;
}
HRESULT ManagementSS::initSSWrap( ID3D11Device* p_device )
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD			= 0;
	sampDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	hr = p_device->CreateSamplerState(&sampDesc, &m_samplerStates[SSTypes_WRAP]);

	if(FAILED(hr))
	{
		MessageBox( NULL,
			L"ManagementSS::initSSDefault() | device->CreateSamplerState() | Failed",
			L"ssDefault", MB_OK | MB_ICONEXCLAMATION );
	} 
	return hr;
}