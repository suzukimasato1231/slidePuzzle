#pragma once
#include<vector>
#include<string>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include "Pipeline.h"
#include"Vec.h"
#include"Singleton.h"
#include<Texture.h>
extern const int window_width;
extern const int window_height;
/// <summary>
/// �X�v���C�g�f�[�^
/// </summary>
struct SpriteData
{
	//���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//Z�����̉�]�p
	float rotation = 0.0f;
	//���W
	Vec3 position = { 0,0,0 };
	//���[���h�s��
	XMMATRIX matWorld;
	//�e�N�X�`���ԍ�
	TextureData texNumber = {};
	//�傫��
	Vec2 size = { 100,100 };

	Vec2 texLeftTop = { 0,0 };

	Vec2 texSize = { 0,0 };
	//�A���J�[�|�C���g
	Vec2 anchorpoint = { 0.5f,0.5f };
	//�e
	SpriteData* parent = nullptr;
};
/// <summary>
/// �QD�X�v���C�g�N���X
/// </summary>
class Sprite :public Singleton<Sprite>
{
	friend Singleton<Sprite>;
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMMATRIX = DirectX::XMMATRIX;
public://�\����
	/// <summary>
	/// ���_�f�[�^
	/// </summary>
	struct VertexPosUv
	{
		Vec3 pos;//xyz���W
		Vec2 uv;//uv���W
	};
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		Vec4 color;
		XMMATRIX mat;//3D�ϊ��s��
	};

	//�X�v���C�g���ʃf�[�^
	struct SpriteCommon
	{
		//�p�C�v���C���Z�b�g
		Pipeline::PipelineSet pipelineSet;
		////�ˉe�s��
		XMMATRIX matProjection{};
	};
	/// <summary>
	/// �R���X�g�o�b�t�@
	/// </summary>
	struct ConstBuffer
	{
		//GPU���\�[�X�̐���1
		ComPtr<ID3D12Resource> constBuff;
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Sprite();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~Sprite();
public:
	static ID3D12Device* dev;
	static ID3D12GraphicsCommandList* cmdList;
	//�X�v���C�g���ʃf�[�^
	SpriteCommon spriteCommon;
public://�ÓI�֐�
	//�X�v���C�g�ݒ�ÓI������
	static void StaticInit(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// Sprite�N���X����
	/// </summary>
	/// <returns></returns>
	static Sprite* Sprite::Create();
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�v���C�g������
	/// </summary>
	/// <param name="widow_width">���T�C�Y</param>
	/// <param name="window_height">�c�T�C�Y</param>
	void SpriteAllInitialization(int widow_width, int window_height);

	//�X�v���C�g���ʃf�[�^�𐶐�
	SpriteCommon SpriteCommonCreate(int window_width, int window_height);
public:
	//�X�v���C�g����
	SpriteData SpriteCreate(const wchar_t* filename);

	//�X�v���C�g�P�̒��_�o�b�t�@�̓]�� �傫����ς����
	void SpriteTransferVertexBuffer(const SpriteData& sprite);
	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();
	/// <summary>
	/// �R���X�g�o�b�t�@�쐬
	/// </summary>
	void CreateConstBuffer();

	//�X�v���C�g�P�̍X�V
	void Update(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY);

	/// <summary>
	/// //�X�v���C�g�P�̕`��
	/// </summary>
	/// <param name="sprite">�X�v���C�g�f�[�^</param>
	/// <param name="position">���W</param>
	/// <param name="width">���̒���</param>
	/// <param name="height">�c�̒���</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="color">�F</param>
	/// <param name="isFlipX"></param>
	/// <param name="isFlipY"></param>
	void Draw(SpriteData& sprite,const Vec2 &position,const float width,const float height,const Vec2 &anchorpoint = { 0.0f,0.0f },const Vec4 &color = { 1.0f,1.0f,1.0f,1.0f },const bool isFlipX = false,const bool isFlipY = false);

	//���|�X�g�G�t�F�N�g
	void PostEffectDraw(ID3D12DescriptorHeap* descHeap, SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint = { 0.0f,0.0f }, const Vec4 &color = { 1.0f,1.0f,1.0f,1.0f }, bool isFlipX = false, bool isFlipY = false);
	//�X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
	void SpriteCommonBeginDraw();
public://�f�o�b�N�e�L�X�g
	void DebugUpdate(SpriteData& sprite);
	//�X�v���C�g�P�̕`��/�X�v���C�g�f�[�^sprite
	void DebugDraw(SpriteData& sprite);
private:
	std::vector<ConstBuffer*>constBuffer;
public:
	UINT spriteNum = 0;
};
