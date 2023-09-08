#pragma once
#include"Vec.h"
/// <summary>
//�C�[�W���O
//��Ԋ֐�
//���`���
/// </summary>
class Easing
{
public:
	/// <summary>
	/// ���
	/// </summary>
	/// <param name="start">�X�^�[�g�ʒu</param>
	/// <param name="end">�I���ʒu</param>
	/// <param name="t">�o�ߎ���</param>
	/// <returns></returns>
	static const Vec3 lerp(const Vec3 &start, const Vec3 &end, const float t);
	/// <summary>
	/// ���X�ɑ���
	/// </summary>
	/// <param name="start">�X�^�[�g�ʒu</param>
	/// <param name="end">�I���ʒu</param>
	/// <param name="t">�o�ߎ���</param>
	/// <returns></returns>
	static const Vec3 easeIn(const Vec3 &start, const Vec3 &end, const float t);
	/// <summary>
	/// ���X�ɒx��
	/// </summary>
	/// <param name="start">�X�^�[�g�ʒu</param>
	/// <param name="end">�I���ʒu</param>
	/// <param name="t">�o�ߎ���</param>
	/// <returns></returns>
	static const Vec3 easeOut(const Vec3 &start, const Vec3 &end, const float t);
	/// <summary>
	/// �ŏ��������X�ɒx��
	/// </summary>
	/// <param name="start">�X�^�[�g�ʒu</param>
	/// <param name="end">�I���ʒu</param>
	/// <param name="t">�o�ߎ���</param>
	/// <returns></returns>
	static const Vec3 easeInOut(const Vec3 &start, const Vec3 &end, const float t);
};

class EaseData
{
public:
	EaseData(int count = 0);
	~EaseData() = default;
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

public:
	/// <summary>
	/// �I��������
	/// </summary>
	/// <returns></returns>
	bool GetEndFlag() { return endFlag_; }
	/// <summary>
	/// �s�����Ă��邩
	/// </summary>
	/// <returns></returns>
	bool GetActFlag() { return actFlag_; }
	/// <summary>
	/// �i�s�x�̎擾
	/// </summary>
	/// <returns>0.0 ~ 1.0</returns>
	float GetTimeRate() { return timeRate_; }
	/// <summary>
	/// �t���[���̐ݒ�
	/// </summary>
	/// <param name="count"></param>
	void SetCount(int count) { countNum_ = count; }
	/// <summary>
	/// �I���t���O�̐ݒ�
	/// </summary>
	/// <param name="Flag"></param>
	void SetEndFlag(bool flag) { endFlag_ = flag; }
	/// <summary>
	/// �s���t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetActFlag(bool flag) { actFlag_ = flag; }
private:
	// �C�[�W���O�̐i�s�x�p
	float timer_ = 0.0f;
	float timeRate_ = 0.0f;
	int countNum_ = 0;
	bool endFlag_ = false;
	bool actFlag_ = true;
};