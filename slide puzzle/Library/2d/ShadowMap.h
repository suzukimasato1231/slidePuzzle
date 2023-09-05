#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include"Vec.h"
#include"DirectXBase.h"
/// <summary>
/// �V���h�E�}�b�v
/// </summary>
class ShadowMap
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos;			//xyz���W
		DirectX::XMFLOAT2 uv;			//uv���W
	};

	struct ConstantBuffer_b0
	{
		DirectX::XMMATRIX mat;			//�ϊ��s��
	};
private:
	static ID3D12Device* dev;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> m_texbuff;
	//SRv�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> m_descHeapSRV;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> m_depthbuff;
	//SRv�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> m_depthHeapSRV;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> m_descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> m_descHeapDSV;
	//�O���t�B�b�N�X�p�C�v���C��
	ComPtr<ID3D12PipelineState> m_pipelineState;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> m_rootSignature;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> m_vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW m_vbView{};
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> m_constBuff;
	//�摜����
	const int c_texture_width = 2500;	
	//�摜�c��
	const int c_texture_height = 2500;	
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ShadowMap();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~ShadowMap();
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �`��O
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void PreDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �`���
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void PostDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	///�[�x�o�b�t�@���擾
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetTexbuff() { return m_depthbuff.Get(); }
private:
	/// <summary>
	/// �p�C�v���C���쐬
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	void CreateGraphicsPipelineState(ID3D12Device* dev);
};