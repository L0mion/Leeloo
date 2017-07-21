#include "Camera.h"
#include "MathHelper.h"
#include <Windows.h>

Camera::Camera(float p_aspect, float p_fov, float p_zFar, float p_zNear)
{
	m_aspect = p_aspect;
	m_fov	 = p_fov;
	m_zFar	 = p_zFar;
	m_zNear	 = p_zNear;

	m_velocity	= 0.01f;

	//m_position	= DirectX::XMFLOAT3(0.0f, 0.0f, -25.0f);
	//m_right	= DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	//m_up	= DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	//m_look	= DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_position	= DirectX::XMFLOAT3(29.0856f, 4.34502f, -29.0478f);
	m_right	= DirectX::XMFLOAT3(0.810962f, 0.0f, 0.585098f);
	m_up	= DirectX::XMFLOAT3(-0.136236f, 0.972514f, 0.188826f);
	m_look	= DirectX::XMFLOAT3(-0.569016f, -0.232844f, 0.788673f);
}

Camera::~Camera()
{
}

void Camera::updateView()
{
	DirectX::XMVECTOR vLook	 = DirectX::XMLoadFloat3(&m_look);
	DirectX::XMVECTOR vUp	 = DirectX::XMLoadFloat3(&m_up);
	DirectX::XMVECTOR vRight = DirectX::XMLoadFloat3(&m_right);

	vLook = DirectX::XMVector3Normalize(vLook);

	vUp = DirectX::XMVector3Cross(vLook, vRight);
	vUp = DirectX::XMVector3Normalize(vUp);

	vRight = DirectX::XMVector3Cross(vUp, vLook);
	vRight = DirectX::XMVector3Normalize(vRight);

	DirectX::XMVECTOR vPosition = DirectX::XMVectorSet( m_position.x*-1.0f,
		m_position.y*-1.0f,
		m_position.z*-1.0f,
		1.0f);

	DirectX::XMVECTOR vResult;
	DirectX::XMFLOAT3 fResult;
	vResult = DirectX::XMVector3Dot(vPosition, vRight);
	DirectX::XMStoreFloat3(&fResult, vResult);
	float x = fResult.x;
	vResult = DirectX::XMVector3Dot(vPosition, vUp);
	DirectX::XMStoreFloat3(&fResult, vResult);
	float y = fResult.y;
	vResult = DirectX::XMVector3Dot(vPosition, vLook);
	DirectX::XMStoreFloat3(&fResult, vResult);
	float z = fResult.z;

	DirectX::XMStoreFloat3(&m_right, vRight);
	DirectX::XMStoreFloat3(&m_up, vUp);
	DirectX::XMStoreFloat3(&m_look, vLook);

	m_view(0, 0) = m_right.x;
	m_view(1, 0) = m_right.y;
	m_view(2, 0) = m_right.z;
	m_view(3, 0) = x;

	m_view(0, 1) = m_up.x;
	m_view(1, 1) = m_up.y;
	m_view(2, 1) = m_up.z;
	m_view(3, 1) = y;

	m_view(0, 2) = m_look.x;
	m_view(1, 2) = m_look.y;
	m_view(2, 2) = m_look.z;
	m_view(3, 2) = z;

	m_view(0, 3) = 0.0f;
	m_view(1, 3) = 0.0f;
	m_view(2, 3) = 0.0f;
	m_view(3, 3) = 1.0f;
}

void Camera::updateProj()
{
	ZeroMemory(&m_projection, sizeof(m_projection));

	m_projection(0, 0) = 1/(m_aspect*(tan(m_fov/2)));
	m_projection(1, 1) = 1/(tan(m_fov/2));
	m_projection(2, 2) = m_zFar/(m_zFar - m_zNear);
	m_projection(2, 3) = 1.0f;
	m_projection(3, 2) = (-m_zNear * m_zFar)/(m_zFar - m_zNear);
}

void Camera::strafe(const float p_distance)
{
	m_position.x += m_right.x * p_distance;
//	m_position.y += m_right.y * p_distance;
	m_position.z += m_right.z * p_distance;
}

void Camera::walk(const float p_distance)
{
	m_position.x += m_look.x * p_distance;
//	m_position.y += m_look.y * p_distance;
	m_position.z += m_look.z * p_distance;
}

void Camera::verticalWalk(const float p_distance)
{
	m_position.y += p_distance;
}

void Camera::pitch(const float p_angle)
{

	//Load vectors in to XMVECTORs to utilize SIMD.
	DirectX::XMVECTOR vLook	= DirectX::XMLoadFloat3(&m_look);
	DirectX::XMVECTOR vUp	= DirectX::XMLoadFloat3(&m_up);

	DirectX::XMVECTOR vStraightUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//Create a Quaternian that describes the rotation.
	float cosAngle = cos((p_angle)/2);
	float sinAngle = sin((p_angle)/2);
	DirectX::XMFLOAT4 fQuaternion = DirectX::XMFLOAT4(m_right.x*sinAngle, m_right.y*sinAngle, m_right.z*sinAngle, cosAngle);
	DirectX::XMVECTOR vQuaternion = DirectX::XMLoadFloat4(&fQuaternion);
	vQuaternion = DirectX::XMQuaternionNormalize(vQuaternion);

	//Rotate all vectors that are affected by the transform.
	vUp	  = DirectX::XMVector3Rotate(vUp, vQuaternion);
	vLook = DirectX::XMVector3Rotate(vLook, vQuaternion);

	//Check the angle between the camera's up-vector and "straight up".
	DirectX::XMFLOAT3 fResult;
	DirectX::XMVECTOR vResult = DirectX::XMVector3AngleBetweenNormals(vUp, vStraightUp);
	DirectX::XMStoreFloat3(&fResult, vResult);

	//Store the results in member variables if the the angle between the camera's up-vector
	//and "straight up" is less than pi/2. Preventing the camera from flipping up-side down. 
	if(fResult.x < DirectX::XM_PI/2 && fResult.x > -DirectX::XM_PI/2)
	{
		DirectX::XMStoreFloat3(&m_up, vUp);
		DirectX::XMStoreFloat3(&m_look, vLook);
	}
}

void Camera::yaw(const float p_angle)
{
	//Load vectors in to XMVECTORs to utilize SIMD.
	DirectX::XMVECTOR vLook	 = DirectX::XMLoadFloat3(&m_look);
	DirectX::XMVECTOR vUp	 = DirectX::XMLoadFloat3(&m_up);
	DirectX::XMVECTOR vRight = DirectX::XMLoadFloat3(&m_right);

	float cosAngle = cos((p_angle)/2);
	float sinAngle = sin((p_angle)/2);

	//Create a Quaternian that describes the rotation.
	DirectX::XMFLOAT4 fQuaternion = DirectX::XMFLOAT4(0.0f*sinAngle, 1.0f*sinAngle, 0.0f*sinAngle, cosAngle);
	DirectX::XMVECTOR vQuaternion = DirectX::XMLoadFloat4(&fQuaternion);
	vQuaternion = DirectX::XMQuaternionNormalize(vQuaternion);

	//Rotate all vectors that are affected by the transform.
	vRight	= DirectX::XMVector3Rotate(vRight, vQuaternion);
	vUp		= DirectX::XMVector3Rotate(vUp, vQuaternion);
	vLook	= DirectX::XMVector3Rotate(vLook, vQuaternion);

	//Store the results in member variables.
	DirectX::XMStoreFloat3(&m_right, vRight);
	DirectX::XMStoreFloat3(&m_up, vUp);
	DirectX::XMStoreFloat3(&m_look, vLook);
}

void Camera::setHeight(float p_height)
{
	m_position.y = p_height;
}

void Camera::setPosition(DirectX::XMFLOAT3 p_position)
{
	m_position = p_position;
}

void Camera::setRotationToZero()
{
	m_right	= DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_up	= DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_look	= DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
}

void Camera::setAspectRatio(float p_aspectRatio)
{
	m_aspect = p_aspectRatio;
}

DirectX::XMFLOAT3 Camera::getPosition()
{
	return m_position;
}

DirectX::XMFLOAT3 Camera::getRight()
{
	return m_right;
}

DirectX::XMFLOAT3 Camera::getUp()
{
	return m_up;
}

DirectX::XMFLOAT3 Camera::getLook()
{
	return m_look;
}

DirectX::XMFLOAT4X4 Camera::getView()
{
	return m_view;
}

DirectX::XMFLOAT4X4 Camera::getViewInv()
{
	return MathHelper::matrixInverse(m_view);
}

DirectX::XMFLOAT4X4 Camera::getProjection()
{
	return m_projection;
}

DirectX::XMFLOAT4X4 Camera::getProjectionInv()
{
	return MathHelper::matrixInverse(m_projection);
}

DirectX::XMFLOAT4X4 Camera::getViewProj()
{
	DirectX::XMMATRIX xmView = DirectX::XMLoadFloat4x4(&m_view);
	DirectX::XMMATRIX xmProj = DirectX::XMLoadFloat4x4(&m_projection);
	DirectX::XMMATRIX xmResult = DirectX::XMMatrixMultiply(xmView, xmProj);
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(&result, xmResult);
	return result;
}