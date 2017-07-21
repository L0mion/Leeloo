#include "ShaderSet.h"
#include "Utility.h"

ShaderSet::ShaderSet()
{
	m_vs			= nullptr;
	m_ps			= nullptr;
	m_cs			= nullptr;
	m_vsBlob		= nullptr;
	m_psBlob		= nullptr;
	m_csBlob		= nullptr;
	m_inputLayout	= nullptr;
}
ShaderSet::~ShaderSet()
{
	SAFE_RELEASE(m_vs);
	SAFE_RELEASE(m_ps);
	SAFE_RELEASE(m_cs);
	SAFE_RELEASE(m_vsBlob);
	SAFE_RELEASE(m_psBlob);
	SAFE_RELEASE(m_csBlob);
	SAFE_RELEASE(m_inputLayout);
}

void ShaderSet::set(ID3D11DeviceContext* p_context)
{
	p_context->VSSetShader(m_vs, nullptr, 0);
	p_context->PSSetShader(m_ps, nullptr, 0);
	p_context->CSSetShader(m_cs, nullptr, 0);
	p_context->IASetInputLayout(m_inputLayout);
}

void ShaderSet::unset(ID3D11DeviceContext* p_context)
{
	p_context->VSSetShader(nullptr, nullptr, 0);
	p_context->PSSetShader(nullptr, nullptr, 0);
	p_context->CSSetShader(nullptr, nullptr, 0);
	p_context->IASetInputLayout(nullptr);
}

ID3D11VertexShader*	 ShaderSet::getVS()
{
	return m_vs;
}
ID3D11VertexShader** ShaderSet::getVSPtr()
{
	return &m_vs;
}
ID3D11PixelShader*	 ShaderSet::getPS()
{
	return m_ps;
}
ID3D11PixelShader**	 ShaderSet::getPSPtr()
{
	return &m_ps;
}
ID3D11ComputeShader* ShaderSet::getCS()
{
	return m_cs;
}
ID3D11ComputeShader** ShaderSet::getCSPtr()
{
	return &m_cs;
}

ID3DBlob*  ShaderSet::getVSBlob()
{
	return m_vsBlob;
}
ID3DBlob** ShaderSet::getVSBlobPtr()
{
	return &m_vsBlob;
}
ID3DBlob*  ShaderSet::getPSBlob()
{
	return m_psBlob;
}
ID3DBlob** ShaderSet::getPSBlobPtr()
{
	return &m_psBlob;
}
ID3DBlob* ShaderSet::getCSBlob()
{
	return m_csBlob;
}
ID3DBlob** ShaderSet::getCSBlobPtr()
{
	return &m_csBlob;
}

ID3D11InputLayout*  ShaderSet::getInputLayout()
{
	return m_inputLayout;
}
ID3D11InputLayout** ShaderSet::getInputLayoutPtr()
{
	return &m_inputLayout;
}
