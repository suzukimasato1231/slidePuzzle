#pragma once
#include <d3dx12.h>
#include"Singleton.h"
#include<dxgi1_6.h>
#include "Pipeline.h"
//�e�N�X�`���f�[�^
struct TextureData
{
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	ComPtr<ID3D12Resource> texbuff;
	int s_texNum = 0;
};
/// <summary>
/// �e�N�X�`���N���X
/// </summary>
class Texture :public Singleton<Texture>
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	template <class T> using vector = std::vector<T>;
	friend Singleton<Texture>;
public:
	static ID3D12Device* dev;					//�f�o�C�X
	static const int textureMax = 2024;

	Texture();

	~Texture();
	//������
	void Init(ID3D12Device* dev);
	//�摜�ǂݍ���
	TextureData LoadTexture(const wchar_t* filename);
	//OBJ�摜�ǂݍ���
	TextureData OBJLoadTexture(const std::string& directoryPath, const std::string& filename);

	TextureData FbxLoadTexture(const DirectX::Image* img, CD3DX12_RESOURCE_DESC texresDesc);

	//�e�p�̐[�x�l�ۑ�
	void LoadShadowTexture(ID3D12Resource* texbuff);
	//��ʊE�[�x�p
	void LoadCameraTexture(ID3D12Resource* texbuff);
	//�|�X�g�G�t�F�N�g�ۑ�
	void LoadPostEfectTexture(ID3D12Resource* texbuff);
	//desc�l��
	ID3D12DescriptorHeap* GetDescHeap();

	TextureData GetShadowTexture()const { return m_shadowData; }

	TextureData GetCameraDepth() const { return m_cameraDepth; }

	TextureData GetPostEfect() const { return m_postEfect; }
	//���摜
	TextureData GetWhite()const { return m_white; }
private:
	//�萔�o�b�t�@�p�̃f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap >m_descHeap = { nullptr };

	//�e�N�X�`���f�[�^�̍쐬
	int m_texNum = 0;
	//���摜�ۑ��p
	TextureData m_white = {};
	//�e�ۑ��p
	TextureData m_shadowData = {};
	//�|�X�g�G�t�F�N�g�ۑ�
	TextureData m_postEfect = {};
	//�J�����[�x
	TextureData m_cameraDepth = {};
};