#ifndef LEELOO_CAMERA_H
#define LEELOO_CAMERA_H

#include <DirectXMath.h>

class Camera
{
public:

	Camera(float p_aspect, float p_fov = 0.785f, float p_zFar = 50.0f, float p_zNear = 0.01f);

	~Camera();

	void updateView();
	void updateProj();

	void strafe(const float p_distance);
	void walk(const float p_distance);
	void verticalWalk(const float p_distance);
	void pitch(const float p_angle);
	void yaw(const float p_angle);

	void setHeight(float p_height);
	void setPosition(DirectX::XMFLOAT3 p_position);
	void setRotationToZero();
	void setAspectRatio(float p_aspectRatio);

	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getRight();
	DirectX::XMFLOAT3 getUp();
	DirectX::XMFLOAT3 getLook();
	DirectX::XMFLOAT4X4 getView();
	DirectX::XMFLOAT4X4 getViewInv();
	DirectX::XMFLOAT4X4 getProjection();
	DirectX::XMFLOAT4X4 getProjectionInv();
	DirectX::XMFLOAT4X4 getViewProj();
private:
	float m_fov;	
	float m_aspect;	
	float m_zNear;	
	float m_zFar;	

	float m_velocity;	
	DirectX::XMFLOAT3 m_position;	
	DirectX::XMFLOAT3 m_right;	
	DirectX::XMFLOAT3 m_up;	
	DirectX::XMFLOAT3 m_look;	

	DirectX::XMFLOAT4X4 m_view;	
	DirectX::XMFLOAT4X4 m_projection;
};

#endif //LEELOO