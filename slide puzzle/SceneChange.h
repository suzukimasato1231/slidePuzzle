#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Sprite.h"

namespace
{
	//�t�F�[�h�^�C�v
	enum class TYPE
	{
		FadeOut = -1,
		Not = 0,
		FadeIn = 1,
	};

	//�t�F�[�h�^�C�v
	enum class SIZEMAX
	{
		Width = 32,
		Height = 18,
	};

	class BlackBox
	{
	public: // �����o�֐�
		/// <summary>
		/// ����
		/// </summary>
		/// <param name="type">�t�F�[�h�^�C�v</param>
		void Spin(TYPE type);
		/// <summary>
		/// �I�t�Z�b�g�^�C�}�[�̐ݒ�
		/// </summary>
		/// <param name="num"></param>
		void AddOffsetTimer(int num) { offsetTimer_ += num; }

	public: // �����o�֐�
		/// <summary>
		/// �I�t�Z�b�g�^�C�}�[�̎擾
		/// </summary>
		/// <returns>�I�t�Z�b�g�^�C�}�[</returns>
		int GetOffsetTimer() { return offsetTimer_; }

	public: // �����o�ϐ�
		SpriteData sprite_;
		Vec2 pos_ = {};
		Vec2 scale_ = {};
		int offsetTimer_ = 0;
		// �C�[�W���O�̐i�s�x�p
		float easeTimer_ = 0.0f;
		bool inEnd_ = false;
		bool outEnd_ = false;
	};
}

class SceneChange
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // �T�u�N���X


public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SceneChange();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �V�[���`�F���W�J�n
	/// </summary>
	/// <param name="sceneName">�V�[����</param>
	void SceneChangeStart(const std::string& sceneName);

public: // �����o�֐�
	/// <summary>
	/// �V�[���C���I���t���O�̎擾
	/// </summary>
	/// <returns>�V�[���C���I���t���O</returns>
	bool GetinEndFlag() { return inEndFlag_; }
	/// <summary>
	/// �V�[���A�E�g�I���t���O�̎擾
	/// </summary>
	/// <returns>�V�[���A�E�g�I���t���O</returns>
	bool GetOutEndFlag() { return outEndFlag_; }

private: // �����o�ϐ�
	bool inEndFlag_ = false;
	bool outEndFlag_ = false;

	std::vector<std::vector<std::unique_ptr<BlackBox>>> blackBox_;
	TYPE type_;
	std::string sceneName_;
};