#pragma once
#include <d3dcompiler.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;
/// <summary>
/// �V�F�[�_�[�N���X
/// </summary>
class Shader
{
public:
	ComPtr<ID3DBlob> vsBlob = nullptr; //���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob = nullptr; //�W�I���g���V�F�[�_�[
	ComPtr<ID3DBlob> psBlob = nullptr; //�s�N�Z���V�F�[�_�I�u�W�F�N�g
};

ComPtr<ID3DBlob> LoadShader(const LPCWSTR &VshaderName = L"", const LPCSTR &Vtarget = "", const LPCSTR &entryPoint = "main");

class ShaderManager
{
public:
	/// <summary>
	/// �X�v���C�g
	/// </summary>
	static Shader spriteShader;		//�X�v���C�g
	/// <summary>
	/// �|�X�g�G�t�F�N�g
	/// </summary>
	static Shader postNormalShader;	//�|�X�g�G�t�F�N�g�i�ʏ�j
	static Shader postTestShader;	//�u���[��
	static Shader ShadowMapShader;	//�e�V�F�[�_�[
	static Shader DepthOfFieldShader;//��ʊE�[�x
	/// <summary>
	/// �I�u�W�F�N�g
	/// </summary>
	static Shader objShader;		//�ʏ�OBJ
	static Shader noShadowOBJShader;//�e����OBJ
	static Shader toonObjShader;	//�g�D�[���V�F�[�_�[
	static Shader normalMapShader;	//�m�[�}���}�b�v
	/// <summary>
	/// �p�[�e�B�N��
	/// </summary>
	static Shader particleShader;	//�p�[�e�B�N��

	static void LoadShaders();
};