#ifndef LEELOO_SHADER_SET_H
#define LEELOO_SHADER_SET_H

#include <d3d11.h>

class ShaderSet
{
public:
	ShaderSet();
	~ShaderSet();

	void set(ID3D11DeviceContext* p_context);
	void unset(ID3D11DeviceContext* p_context);

	ID3D11VertexShader*		getVS();
	ID3D11VertexShader**	getVSPtr();
	ID3D11PixelShader*		getPS();
	ID3D11PixelShader**		getPSPtr();
	ID3D11ComputeShader*	getCS();
	ID3D11ComputeShader**	getCSPtr();

	ID3DBlob*  getVSBlob();
	ID3DBlob** getVSBlobPtr();
	ID3DBlob*  getPSBlob();
	ID3DBlob** getPSBlobPtr();
	ID3DBlob*  getCSBlob();
	ID3DBlob** getCSBlobPtr();

	ID3D11InputLayout*  getInputLayout();
	ID3D11InputLayout** getInputLayoutPtr();
private:
	ID3D11VertexShader*  m_vs;
	ID3D11PixelShader*	 m_ps;
	ID3D11ComputeShader* m_cs;

	ID3DBlob* m_vsBlob;
	ID3DBlob* m_psBlob;
	ID3DBlob* m_csBlob;

	ID3D11InputLayout* m_inputLayout;
};

#endif //LEELOO_SHADER_SET_H