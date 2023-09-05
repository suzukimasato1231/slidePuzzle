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