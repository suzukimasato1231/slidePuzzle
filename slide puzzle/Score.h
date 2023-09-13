#pragma once
#include"Object.h"
#include"Sprite.h"
#include <array>
#include <algorithm>
#include <functional>

class Score final
{
private:
	Score() = default;
	~Score() = default;

public:
	Score(const Score& r) = delete;
	Score& operator=(const Score& r) = delete;

	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static Score* GetInstance();

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(Vec2 pos = {});

	void Reset();

	void CrstalGet(Vec3 position);
	int GetComboTime() { return comboTime; }
	void SetIsScoreDraw(bool flag) { isScoreDraw = flag; }

private:
	SpriteData backSprite;
	SpriteData comboSprite;
	SpriteData numberGraph[10] = {};
	const int comboTimeMax = 250;//�R���{�ő�
	int comboTime = 0;           //�R���{����
	int pointNum = 0;            //���_
	int pointPlas = 10;          //���_���Z
	TextureData particleGraph = {};

	//�X�R�A�`��
	ObjectData scoreDraw = {};
	SpriteData comboGraph[11] = {};
	Vec3 scorePos = {};
	bool isScoreDraw = false;
	const int scoreDrawTimeMax = 50;
	int scoreDrawTime = 0;
};

