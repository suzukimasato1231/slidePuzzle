#include "SceneManager.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"
#include"PostEffect.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"StageSelect.h"
#include"ResultScene.h"
SceneManager::SceneManager()
{}
SceneManager::~SceneManager()
{
	//XAudio2���
	Audio::Get()->xAudio2.Reset();
	//���f�[�^���
	scene_->Finalize();
	delete scene_;
}
void SceneManager::Initialize()
{
	//�ėp�@�\
	//�Q�[���E�B���h�E�̍쐬
	Window::Get()->CreateGameWindow();
	// DirectX����������
	DirectXBase::Get()->Initilize();
	//���͂̏�����
	Input::Get()->Initialize();
	//�V�F�[�_�[�̓ǂݍ���
	ShaderManager::LoadShaders();
	//���C�g�ÓI������
	LightGroup::StaticInitialize(DirectXBase::Get()->GetDevice());
	//�X�v���C�g�ÓI������
	Sprite::StaticInit(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList());
	//�e�L�X�g�N���X������
	Texture::Get()->Init(DirectXBase::Get()->GetDevice());
	Pipeline::CreatePipeline(DirectXBase::Get()->GetDevice());
	//�f�o�b�N�e�L�X�g������
	DebugText::Get()->Initialize();
	//�X�v���C�g�N���X�쐬
	Sprite::Get()->Init();
	//FBX������
	FbxLoader::GetInstance()->Initialize(DirectXBase::Get()->GetDevice());
	FBXObject3d::SetDevice(DirectXBase::Get()->GetDevice());
	FBXObject3d::SetCmdList(DirectXBase::Get()->GetCmandList());
	FBXObject3d::CreateGraphicsPipeline();
	FBXObject3d::CreateShadowPipeline();
	//�}�`���f��������
	Shape::Init(DirectXBase::Get()->GetDevice());
	//�p�[�e�B�N��������
	ParticleManager::StaticInitialize(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList(), window_width, window_height);
	//3D�I�u�W�F�N�g������
	Object::Init(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList());

	PostEffect::Get()->Initialize(DirectXBase::Get()->GetDevice());
	////�e
	m_shadowMapFar.Init();
	Texture::Get()->LoadShadowTexture(m_shadowMapFar.GetTexbuff());
	Texture::Get()->LoadCameraTexture(PostEffect::Get()->GetDepth());

	//���쐬
	Audio::Get()->Init();
	
	//�V�[�����^�C�g���ɐݒ�
	BaseScene* scene = new GameScene();
	SetNextScene(scene);
}

void SceneManager::Update()
{
	Input::Get()->Update();

	//�V�[���`�F���W
	SceneChange();

	//���̃V�[���̗\�񂪂���Ȃ�
	if (nextScene_)
	{
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}
		//�V�[���؂�ւ�
		scene_ = nextScene_;
		nextScene_ = nullptr;

		scene_->SetSceneManager(this);
		//���̃V�[��������������
		scene_->Init();
	}
	//�X�V
	scene_->Update();

	if (Input::Get()->KeybordTrigger(DIK_T) == true)
	{
		BaseScene* scene = new TitleScene();
		SetNextScene(scene);
	}
}

void SceneManager::Draw()
{
	//�e�[�x�l�擾
	m_shadowMapFar.PreDraw(DirectXBase::Get()->GetCmandList());
	Object::InitDraw();
	//�e�`��
	scene_->ShadowDraw();
	m_shadowMapFar.PostDraw(DirectXBase::Get()->GetCmandList());


	PostEffect::Get()->PreDrawScene(DirectXBase::Get()->GetCmandList());
	Object::InitDraw(), Sprite::Get()->PreDraw();
	//�J�����ڐ��̕`��
	scene_->Draw();
	DebugText::Get()->DrawAll();
	PostEffect::Get()->PostDrawScene(DirectXBase::Get()->GetCmandList());

	DirectXBase::Get()->PreDraw();
	////�|�X�g�G�t�F�N�g�̕`��
	PostEffect::Get()->Draw(DirectXBase::Get()->GetCmandList());
	DirectXBase::Get()->ResourceBarrier();
}

void SceneManager::Delete()
{
	Object::Delete();
}

void SceneManager::SceneChange()
{
	
}