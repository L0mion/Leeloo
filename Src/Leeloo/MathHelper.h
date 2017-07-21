#ifndef LEELOO_DIRECTX_MATH_HELPER_H
#define LEELOO_DIRECTX_MATH_HELPER_H

#include <DirectXMath.h>

class MathHelper
{
public:
	static DirectX::XMFLOAT4X4 matrixInverse(DirectX::XMFLOAT4X4 p_matrix);
};

#endif //LEELOO_DIRECTX_MATH_HELPER_H