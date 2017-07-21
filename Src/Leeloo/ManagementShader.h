#ifndef LEELOO_MANAGEMENT_SHADER_H
#define LEELOO_MANAGEMENT_SHADER_H

#include <string>

#include <d3d11.h>
#include <d3dcompiler.h>
#include "ShaderIds.h"

class ShaderSet;

class ManagementShader
{
public:
	ManagementShader(void);
	~ManagementShader(void);

	void setShader(ID3D11DeviceContext* p_context, ShaderIds::Id p_shaderId);
	void unsetShader(ID3D11DeviceContext* p_context);

	HRESULT init(ID3D11Device* p_device);
private:
	HRESULT initShaders(ID3D11Device* p_device);
	HRESULT initVertexShader(ID3D11Device* p_device, std::wstring p_filePath, 
		ID3D11VertexShader** io_vs, ID3DBlob** io_blob);
	HRESULT initPixelShader(ID3D11Device* p_device, std::wstring p_filePath,
		ID3D11PixelShader** io_ps, ID3DBlob** io_blob);
	HRESULT initComputeShader(ID3D11Device* p_device, std::wstring p_filePath,
		ID3D11ComputeShader** io_cs, ID3DBlob** io_blob);

	HRESULT initInputLayouts(ID3D11Device* p_device);

	ShaderSet* m_shaders[ShaderIds::ShaderIds_COUNT];
};

#endif //LEELOO_MANAGEMENT_SHADER_H