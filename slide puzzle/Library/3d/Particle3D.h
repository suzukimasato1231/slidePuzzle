#pragma once
#include <Object.h>
//3D�p�[�e�B�N��
class Particle3D
{
private:
	struct ParticleData
	{
		ObjectData objData = {};
		PSR psr = {};
		Vec3 position = {};
		Vec3 velocity = {};
		Vec3 accel = {};
		Vec3 angle = {};
		Vec3 anglePlas = {};
		TextureData graph = {};
		int framNow = 0;
		int framMax;
	};
public:
	//�R���X�g���N�^
	Particle3D();
	//�f�X�R���X�g���N�^
	~Particle3D();
	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�쐬
	void Create(const ObjectData& objData, const Vec3& pos, const  Vec3& velocity,
		const  Vec3& accel, const Vec3& anglePlas, const  int time, const TextureData &graph = {});

	static Particle3D* Create();
private:
	std::vector<ParticleData*>particle;
};


