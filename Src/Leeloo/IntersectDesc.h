#ifndef LEELOO_INTERSECT_DESC_H
#define LEELOO_INTERSECT_DESC_H

#include <DirectXMath.h>

struct IntersectDesc
{
	DirectX::XMFLOAT2 m_barCoord;
	float m_range;
	int m_triangleId;
	int m_sphereId;

	IntersectDesc::IntersectDesc()
	{
		m_barCoord	 = DirectX::XMFLOAT2(0.0f, 0.0f);
		m_range		 = 0.0f;
		m_triangleId = -1;
		m_sphereId	 = -1;
	}
};

#endif //RAYTRACER_INTERSECT_DESC_H