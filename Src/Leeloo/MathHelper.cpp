#include "MathHelper.h"

DirectX::XMFLOAT4X4 MathHelper::matrixInverse(DirectX::XMFLOAT4X4 p_matrix)
{
	DirectX::XMMATRIX xmMatrix		= DirectX::XMLoadFloat4x4(&p_matrix);
	DirectX::XMVECTOR xmDeterminant = DirectX::XMMatrixDeterminant(xmMatrix);
	DirectX::XMMATRIX xmMatrixInv	= DirectX::XMMatrixInverse(&xmDeterminant, xmMatrix);
	DirectX::XMFLOAT4X4 matrixInv;
	DirectX::XMStoreFloat4x4(&matrixInv, xmMatrixInv);
	return matrixInv;
}