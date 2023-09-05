#pragma once
#include<vector>
#include<string>
#include<d3dx12.h>
#include"Pipeline.h"
#include"Camera.h"
#include<unordered_map>
#include"LightGroup.h"
#include"Vec.h"
#include"Singleton.h"
#include"Texture.h"
#pragma comment(lib,"d3d12.lib")

////���_�f�[�^�\����
struct Vertex
{
	Vec3 pos;//xyz���W
	Vec3 normal;//�@���x�N�g��
	Vec2 uv;//uv���W
};

//�}�e���A��
struct Material
{
	std::string name;			//�}�e���A����
	Vec3 ambient;			//�A���r�G���g�e���x
	Vec3 diffuse;			//�f�B�t���[�Y�e���x
	Vec3 specular;			//�X�y�L�����[�e���x
	float alpha;				//�A���t�@
	std::string textureFilename;//�e�N�X�`���t�@�C����
	//�R���X�g���N�^
	Material()
	{
		ambient = { 1.0f,1.0f,1.0f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;
	}
};
/// <summary>
/// �ʒu�A�傫���A��]���
/// </summary>
struct PSR
{
	Vec3 position = {};
	Vec3 scale = {};
	Vec3 rotation = {};
	//���[���h�ϊ�
	XMMATRIX matWorld;
	//�e�N���X
	PSR* parent = nullptr;
	std::string fileName;
};

//�I�u�W�F�N�g�f�[�^�\����
struct ObjectData
{
	//���_�f�[�^
	std::vector<Vertex*> vertices;
	//�C���f�b�N�X�f�[�^
	std::vector<unsigned short*> indices;
	//���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//�I�u�W�F�ǂݍ��ݗp
	TextureData OBJTexture = {};
	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>>smoothData;
	//�}�e���A��
	Material material;

	int indicesNum = 0;

	//���[���h�ϊ�
	XMMATRIX matWorld;
	//�e�N���X
	ObjectData* parent = nullptr;
};
/// <summary>
/// 3D�I�u�W�F�N�g�N���X
/// </summary>
class Object :public Singleton<Object>
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	friend Singleton<Object>;
public:
	static ID3D12Device* dev;					//�f�o�C�X
	static ID3D12GraphicsCommandList* cmdList;	//�R�}���h���X�g
public://�\����

	struct ConstBuffer
	{
		//GPU���\�[�X�̐���
		ComPtr<ID3D12Resource>constBuff = nullptr;
	};

	//OBJ�f�[�^�֘A
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h���W
		Vec3 cameraPos;		//�J�������W(���[���h���W)
		Vec2 uv;
		XMMATRIX lightproj;//�r���[�v���W�F�N�V�����s��
	};
	struct ConstBufferDataB1
	{
		Vec3 ambient;	//�A���r�G���g�W��
		float pad1;			//�p�f�B���O
		Vec3 diffuse;	//�f�B�t���[�Y�W��
		float pad2;			//�p�f�B���O
		Vec3 specular;	//�X�y�L�����[�W��
		float alpha;		//�A���t�@
	};

	//OBJ�萔�o�b�t�@
	struct OBJBuffer
	{
		ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
		ComPtr<ID3D12Resource> constBuffB1; // �萔�o�b�t�@
	};
public://�ϐ�	
	//OBJ�f�[�^
	static std::vector<OBJBuffer*> OBJbuffer;
public:
	Object() {};

	~Object();
	//������
	static void Init(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static Object* Create();

	static void SetLight(LightGroup* light) { Object::lightGroup = light; }

	static void Delete();
public:
	//���C�g�N���X
	static LightGroup* lightGroup;
public://�I�u�W�F�N�g�֘A
	//�I�u�W�F�N�g�`��O
	static void InitDraw();
	//�萔�o�b�t�@�ݒ�
	static void OBJConstantBuffer();
	/// <summary>
	/// �I�u�W�F�N�g�̃f�[�^�X�V
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="scale">�傫��</param>
	/// <param name="matRot">��]</param>
	/// <param name="color">�F</param>
	static void MatWord(const ObjectData& polygon, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& uv = {});
	/// <summary>
	/// OBJ�`��
	/// </summary>
	/// <param name="polygon">�I�u�W�F�N�g�f�[�^</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�傫��</param>
	/// <param name="matRot">��]</param>
	/// <param name="color">�F</param>
	static	void Draw(const ObjectData& polygon, const Vec3& position, const Vec3& scale, const Vec3& rotation, const TextureData& graph = {}, const Vec2& uv = {});

	/// <summary>
	/// �I�u�W�F�N�g�̃f�[�^�X�V
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="scale">�傫��</param>
	/// <param name="matRot">��]</param>
	/// <param name="color">�F</param>
	static void MatWordUVScroll(const ObjectData& polygon, PSR& psr, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& color);
	/// <summary>
	/// OBJ�`��
	/// </summary>
	/// <param name="polygon">�I�u�W�F�N�g�f�[�^</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�傫��</param>
	/// <param name="matRot">��]</param>
	/// <param name="color">�F</param>
	static	void DrawUVScroll(const ObjectData& polygon, PSR& psr, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& uv = { 0.0f,0.0f }, const TextureData& graph = {});
	/// <summary>
	/// �e�����I�u�W�F�N�g�̃f�[�^�X�V
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="scale">�傫��</param>
	/// <param name="matRot">��]</param>
	/// <param name="color">�F</param>
	static void NoShadowMatWorld(const ObjectData& polygon, PSR& psr, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& uv = {});

	/// <summary>
	/// �e����OBJ�`��
	/// </summary>
	/// <param name="polygon">�I�u�W�F�N�g�f�[�^</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�傫��</param>
	/// <param name="matRot">��]</param>
	/// <param name="color">�F</param>
	static	void NoShadowDraw(const ObjectData& polygon, PSR& psr, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& color = {}, const TextureData& graph = {});

public:
	static size_t OBJNum;//OBJ�ǂݍ��݂̐�
};