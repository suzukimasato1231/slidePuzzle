#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include"Vec.h"
#include"Singleton.h"
extern const int window_width;
extern const int window_height;
/// <summary>
/// �J�����N���X
/// </summary>
class Camera :public Singleton<Camera>
{
	using XMMATRIX = DirectX::XMMATRIX;
private:
	friend Singleton<Camera>;
	//�r���[�ϊ��s��
	XMMATRIX m_matView;
	//�ˉe�ϊ�
	XMMATRIX m_matProjection;
	
	XMMATRIX m_matViewProjection;

	Vec3 m_eye = {}, m_target = {}, m_up = {};

	//�J�����Ǐ]
	bool m_followDirty = false;
	float m_followX = 0.0f;
	float m_followY = 0.0f;
	Vec3 m_followD = {};
	Vec3 m_followF3 = {};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Camera();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~Camera();

	/// <summary>
	/// �J��������
	/// </summary>
	/// <returns></returns>
	static Camera *Create();

	//�J����������/�J�����ʒueye/�J�����^�[�Q�b�gtarget /�J�����̌���up
	void Initilize(const Vec3 &eye, const Vec3 &target, const Vec3 &up);
	//�J�����ʒu�Z�b�g/�J�����ʒueye/�J�����^�[�Q�b�gtarget /�J�����̌���up
	void SetCamera(const Vec3 &eye, const Vec3 &target, const Vec3 &up);
	/// <summary>
	/// �J�������Ǐ]����
	/// </summary>
	/// <param name="position">�Ǐ]����I�u�W�F�N�g�̍��W</param>
	/// <param name="d">�I�u�W�F�N�g�ƃJ�����̋���</param>
	/// <param name="angleX">�J�����̌���X</param>
	/// <param name="angleY">�J�����̌���Y</param>
	/// <returns></returns>
	void FollowCamera(const Vec3 &position, const Vec3 &d, float angleX = 0, float angleY = 0);

	//matView���l��
	 XMMATRIX GetMatView()const;
	//projection���l��
	 XMMATRIX GetProjection()const;

	 XMMATRIX GetMatViewProjection()const;
	 /// <summary>
	 /// Eye�擾
	 /// </summary>
	 /// <returns></returns>
	 Vec3 GetEye()const;
	 /// <summary>
	 /// �^�[�Q�b�g�擾
	 /// </summary>
	 /// <returns></returns>
	 Vec3 GetTarget()const;
	 /// <summary>
	 /// UP�擾
	 /// </summary>
	 /// <returns></returns>
	 Vec3 GetUp()const;
};