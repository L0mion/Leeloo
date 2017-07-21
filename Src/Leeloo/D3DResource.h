#ifndef LEELOO_D3DRESOURCE_H
#define LEELOO_D3DRESOURCE_H

#include <d3d11.h>

class D3DResource
{
public:
	D3DResource();
	~D3DResource();

	void csSetSRV(ID3D11DeviceContext* p_context, unsigned int p_register);
	void csSetUAV(ID3D11DeviceContext* p_context, unsigned int p_register);

	unsigned int getElementCnt();

	ID3D11ShaderResourceView* getSrv() const;
	ID3D11ShaderResourceView** getSrvPtr();
	ID3D11UnorderedAccessView* getUav() const;
	ID3D11UnorderedAccessView** getUavPtr();

	HRESULT init(ID3D11Device* p_device, unsigned int p_elementSize, unsigned int p_elementCnt, void* p_initData);

private:
	HRESULT createStructuredBuffer(ID3D11Device* p_device, unsigned p_elementSize, unsigned p_elementCnt, void* p_initData);
	HRESULT createShaderResourceView(ID3D11Device* p_device);
	HRESULT createUnorderedAccessView(ID3D11Device* p_device);

	ID3D11Buffer* m_buffer;
	ID3D11ShaderResourceView* m_srv;
	ID3D11UnorderedAccessView* m_uav;
};

#endif //LEELOO_D3DRESOURCE_H