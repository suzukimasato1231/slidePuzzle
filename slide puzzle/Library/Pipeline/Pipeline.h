#pragma once
#include<d3dcompiler.h>
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>
#include"ShaderManager.h"
using namespace Microsoft::WRL;
using namespace DirectX;
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

enum PipelineType
{
	PipelineNormal,
	PipelineNoShadow,
	PipelineFBX,
};

/// <summary>
/// �p�C�v���C���N���X
/// </summary>
class Pipeline
{
public:
	//�\����
	struct PipelineSet
	{
		//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState>pipelinestate;
		//���[�g�V�O�l�`��
		ComPtr <ID3D12RootSignature>rootsignature;
	};
	//�X�v���C�g
	static PipelineSet  SpriteCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
	//OBJ�ǂݍ���
	static PipelineSet  OBJCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);

	static PipelineSet  ShadowCreateGraphicsPipeline(ID3D12Device* dev, const Shader& shader);
	//�e����OBJ
	static PipelineSet  NoShadowOBJCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
	//�m�[�}���}�b�v
	static PipelineSet NormalMapCreatePipeline(ID3D12Device* dev, const Shader &shader);
	//�p�[�e�B�N��
	static PipelineSet ParticleCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
	//�|�X�g�G�t�F�N�g(�ʏ�)
	static PipelineSet PostNormalCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
	//��ʊE�[�x
	static PipelineSet DepthOfFieldPipelineCreateGraphicesPipeline(ID3D12Device* dev, const Shader &shader);
public:
	//�I�u�W�F�N�g�`��
	static PipelineSet OBJPipeline;
	//�e�����I�u�W�F�N�g
	static PipelineSet NoShadowOBJPipeline;
	//�QD�X�v���C�g�`��
	static PipelineSet SpritePipeline;
	//�p�[�e�B�N��
	static PipelineSet ParticlePiepline;
	//�m�[�}���}�b�v�Q���
	static PipelineSet NormalMapPipeline;
	//�|�X�g�G�t�F�N�g
	static PipelineSet PostPipeline;
	//�[�x�l�ۑ��p
	static PipelineSet ShadowMapPipeline;
	//��ʊE�[�x
	static PipelineSet DepthOfFieldPipeline;
	//�p�C�v���C���쐬
	static void CreatePipeline(ID3D12Device* dev);

	static int pipelineType;

	static bool SetPipeline(int type);
};