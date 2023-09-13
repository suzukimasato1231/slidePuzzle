#include <Object.h>
#include<memory>
#include"FBXObject3d.h"
#include<Audio.h>
#include"BaseScene.h"
#include<Sprite.h>
#include"../slide puzzle/SceneChange.h"
/// <summary>
/// �V�[���ړ�
/// </summary>
enum class ResultNext
{
	ResultNextStage,
	ResultSelect,
};

/// <summary>
/// ���U���g�V�[��
/// </summary>
class ResultScene:public BaseScene
{
private:

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ResultScene();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~ResultScene();
	/// <summary>
	/// ���[�v������
	/// </summary>
	void Init()override;
	/// <summary>
	/// �X�V
	/// </summary>
	void Update()override;
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="stageNum">�X�e�[�W</param>
	void Draw()override;
	/// <summary>
	/// �e�`��
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
private:
	std::unique_ptr<LightGroup> lightGroup;
private:
	// �V�[���`�F���W
	std::unique_ptr<SceneChange> sceneChange_;

	SpriteData back;
	SpriteData gameoverGraph;
};