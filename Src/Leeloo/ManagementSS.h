#ifndef LEELOO_MANAGEMENT_SS_H
#define LEELOO_MANAGEMENT_SS_H

#include <vector>
#include <d3d11.h>

class ManagementSS
{
public:
	ManagementSS();
	~ManagementSS();

	enum SSTypes
	{
		SSTypes_DEFAULT,
		SSTypes_WRAP,
		SSTypes_COUNT
	};

	void csSetSS(ID3D11DeviceContext* devcon, SSTypes ssType, unsigned int sahderRegister);

	HRESULT init(ID3D11Device* p_device);
private:
	HRESULT initSSDefault(ID3D11Device* p_device);
	HRESULT initSSWrap( ID3D11Device* p_device );

	ID3D11SamplerState* m_samplerStates[SSTypes_COUNT];
};

#endif //LEELOO_MANAGEMENT_SS_H