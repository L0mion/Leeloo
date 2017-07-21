#ifndef LEELOO_SPHERE_DESC_H
#define LEELOO_SPHERE_DESC_H

#include <DirectXMath.h>

struct SphereDesc
{
	DirectX::XMFLOAT3 center;
	float radius;
	int materialIndex;

	SphereDesc()
	{
		center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		radius = 0.0f;
		materialIndex = -1;
	}
};

#endif //LEELOO_SPHERE_DESC_H