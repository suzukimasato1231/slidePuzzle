#pragma once
#include <Object.h>
//3Dパーティクル
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
	//コンストラクタ
	Particle3D();
	//デスコンストラクタ
	~Particle3D();
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();
	//作成
	void Create(const ObjectData& objData, const Vec3& pos, const  Vec3& velocity,
		const  Vec3& accel, const Vec3& anglePlas, const  int time, const TextureData &graph = {});

	static Particle3D* Create();
private:
	std::vector<ParticleData*>particle;
};


