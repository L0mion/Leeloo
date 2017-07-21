#ifndef LEELOO_MANAGEMENT_CB_H
#define LEELOO_MANAGEMENT_CB_H

#include <d3d11.h>
#include <DirectXMath.h>

struct CBFrame
{
	DirectX::XMFLOAT4X4	viewMatrix;
	DirectX::XMFLOAT4X4	viewMatrixInv;
	DirectX::XMFLOAT4X4	projMatrix;
	DirectX::XMFLOAT4X4	projMatrixInv;
	DirectX::XMFLOAT3	cameraPosition;
	unsigned int		numLights;
};

struct CBWindowResize
{
	unsigned int screenWidth;
	unsigned int screenHeight;

	int padding0;
	int padding1;
};

struct CBObject
{
	unsigned int numTriangles;
	unsigned int numSpheres;
	int padding2;
	int padding3;
};


class ManagementCB
{
public:
	ManagementCB();
	~ManagementCB();

	enum CBIds
	{
		CBIds_FRAME,
		CBIds_WINDOW_RESIZE,
		CBIds_OBJECT,
		CBIds_COUNT
	};

	void vsSetCB(ID3D11DeviceContext* p_context, CBIds p_cbId);
	void psSetCB(ID3D11DeviceContext* p_context, CBIds p_cbId);
	void csSetCB(ID3D11DeviceContext* p_context, CBIds p_cbId);

	void updateCBFrame(ID3D11DeviceContext* p_context, CBFrame p_cb);
	void updateCBWindowResize(ID3D11DeviceContext* p_context, CBWindowResize p_cb);
	void updateCBObject(ID3D11DeviceContext* p_context, CBObject p_cb);

	HRESULT init(ID3D11Device* p_device);
private:
	HRESULT initConstantBuffer(ID3D11Device* p_device, ID3D11Buffer** io_cb, unsigned int p_size);

	ID3D11Buffer* m_buffers[CBIds_COUNT];
};

#endif //LEELOO_MANAGEMENT_CB_H