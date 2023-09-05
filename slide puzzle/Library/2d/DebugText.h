#pragma once
#include "Sprite.h"
#include "Safe_delete.h"
#include <stdio.h>
#include <stdarg.h>
#include<memory>
extern const int window_width;
extern const int window_height;
/// <summary>
/// �f�o�b�N�e�L�X�g
/// </summary>
class DebugText :public Singleton<DebugText>
{
	friend Singleton<DebugText>;
public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 256;	// �ő啶����
	static const int fontWidth = 9;			// �t�H���g�摜��1�������̉���
	static const int fontHeight = 18;		// �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14;	// �t�H���g�摜��1�s���̕�����
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DebugText();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~DebugText();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	///  �f�o�b�N�e�L�X�g
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="size">�T�C�Y</param>
	/// <param name="text">�\�����镶��</param>
	/// <param name=""></param>
	void Print(const float x, const float y, const float size,std::string text, ...);
	/// <summary>
	/// �f�o�b�N�e�L�X�g�S�`��
	/// </summary>
	void DrawAll();
private:
	std::unique_ptr<Sprite> sprite = nullptr;

	// �X�v���C�g�f�[�^�̔z��
	SpriteData m_sprites[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int m_spriteIndex = 0;
};
