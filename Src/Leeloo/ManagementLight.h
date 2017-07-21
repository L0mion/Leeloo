#ifndef LEELOO_MANAGEMENT_LIGHT_H
#define LEELOO_MANAGEMENT_LIGHT_H

#include <vector>

#include <d3d11.h>
#include <DirectXMath.h>

#include "LightDesc.h"

class ManagementLight
{
public:
	ManagementLight();
	~ManagementLight();

	void moveLights(ID3D11DeviceContext* p_context, float p_dt);

	void csSetLightSRV(ID3D11DeviceContext* p_context, unsigned int p_startSlot);

	unsigned int getNumLights();

	HRESULT init(ID3D11Device* p_device);
private:
	void initLights();
	HRESULT initLightBuffer(ID3D11Device* p_device);
	HRESULT initLightSRV(ID3D11Device* p_device);

	HRESULT updateLightBuffer(ID3D11DeviceContext* p_context);

	std::vector<LightDesc> m_lights;
	ID3D11Buffer* m_lightBuffer;
	ID3D11ShaderResourceView* m_lightSRV;

	int m_lightDir;
};

#endif //LEELOO_MANAGEMENT_LIGHT_H