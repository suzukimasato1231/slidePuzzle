#include "Camera.h"
using namespace DirectX;

Camera::Camera()
{
	m_matView = XMMatrixIdentity();
	//射影変換
	m_matProjection = XMMatrixIdentity();
}

Camera::~Camera()
{
}

Camera* Camera::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	Camera* view = new Camera();
	if (view == nullptr) {
		return nullptr;
	}

	// 初期化
	return view;
}

void Camera::Initilize(const Vec3& eye, const Vec3& target, const Vec3& up)
{
	//射影変換用
	m_matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		1.0f, 100.0f
	);
	m_eye = eye, m_target = target, m_up = up;
	m_matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::SetCamera(const Vec3& eye, const Vec3& target, const Vec3& up)
{
	m_eye = eye, m_target = target, m_up = up;
	m_matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//射影変換用
	m_matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		1.0f, 1200.0f
	);
	m_matViewProjection = m_matView * m_matProjection;
}

void Camera::FollowCamera(const Vec3& position, const Vec3& d, float angleX, float angleY)
{
	m_target = position;//注視点座標

   //カメラ追従
	if (angleX != m_followX || angleY != m_followY || !(d == m_followD))
	{
		m_followX = angleX, m_followY = angleY, m_followD = d;
		//1
		Vec3 V0 = d;
		//2
		XMMATRIX followRotM = XMMatrixIdentity();
		followRotM *= XMMatrixRotationY(XMConvertToRadians(angleX));//Y軸
		followRotM *= XMMatrixRotationX(XMConvertToRadians(angleY));//X軸

		//3
		XMVECTOR v3 = { V0.x,V0.y,V0.z };
		XMVECTOR v = XMVector3TransformNormal(v3, followRotM);

		//4
		m_followF3 = { v.m128_f32[0],v.m128_f32[1],v.m128_f32[2] };
	}
	m_eye.x = m_target.x + m_followF3.x, m_eye.y = m_target.y + m_followF3.y, m_eye.z = m_target.z + m_followF3.z;

	//5
	//ビュー変換行列
	m_matView = XMMatrixLookAtLH(XMLoadFloat3(&m_eye), XMLoadFloat3(&m_target), XMLoadFloat3(&m_up));

	m_matViewProjection = m_matView * m_matProjection;
}

XMMATRIX Camera::GetMatView()const
{
	return m_matView;
}

XMMATRIX Camera::GetProjection()const
{
	return m_matProjection;
}

XMMATRIX Camera::GetMatViewProjection()const
{
	return m_matViewProjection;
}

Vec3 Camera::GetEye()const
{
	return m_eye;
}
Vec3 Camera::GetTarget()const
{
	return m_target;
}

Vec3 Camera::GetUp()const
{
	return m_up;
}
