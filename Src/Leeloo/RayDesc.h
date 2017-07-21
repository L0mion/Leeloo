#ifndef LEELOO_RAY_DESC_H
#define LEELOO_RAY_DESC_H

#include <DirectXMath.h>

struct RayDesc
{
	DirectX::XMFLOAT3 m_origin;
	float m_maxDistance;
	DirectX::XMFLOAT3 m_direction;
	int m_triangleId;
	DirectX::XMFLOAT3 m_reflectiveFactor;
	int m_sphereId;

	RayDesc::RayDesc()
	{
		m_origin			= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_maxDistance		= 0;
		m_direction			= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_triangleId		= -1;
		m_reflectiveFactor	= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_sphereId			= -1;
	}
};

#endif //LEELOO_RAY_DESC_H