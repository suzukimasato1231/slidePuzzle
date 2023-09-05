#pragma once
#include "Sprite.h"
#include"Singleton.h"
#include"Sprite.h"
#include"Texture.h"
/// <summary>
/// �|�X�g�G�t�F�N�g
/// </summary>
class PostEffect : public Singleton<PostEffect>
{
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	friend Singleton<PostEffect>;
public:

	//��ʃN���A�J���[
	static const float clearColor[4];
	static ID3D12Device* dev;
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(ID3D12Device* dev);

	//�p�C�v���C������
	void CreatePipeline(ID3D12Device* dev);

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList,const Vec4 &color = { 1.0f,1.0f,1.0f,1.0f });
	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	void UpdatePost(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY);
	/// <summary>
	/// �|�X�g�G�t�F�N�g�`��
	/// </summary>
	/// <param name="sprite">�X�v���C�g�f�[�^</param>
	/// <param name="position">���W</param>
	/// <param name="width">����</param>
	/// <param name="height">�c��</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="color">�F</param>
	/// <param name="isFlipX">�����]</param>
	/// <param name="isFlipY">�c���]</param>
	void DrawPost(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY);
	//�p�C�v���C���؂�ւ�
	void SetPipeline(int num);

	ID3D12Resource* GetDepth() { return m_depthBuff.Get(); }
private:

	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource>m_texbuff;
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>m_descHeapSRV;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource>m_depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>m_descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>m_descHeapDSV;

	//GPU���\�[�X�̐���1
	ComPtr<ID3D12Resource> m_constBuff;

	SpriteData m_sprite;

	//�p�C�v���C���Z�b�g
	Pipeline::PipelineSet m_pipelineSet;
};

