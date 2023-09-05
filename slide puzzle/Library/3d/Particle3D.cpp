#include "Particle3D.h"

Particle3D::Particle3D()
{
}

Particle3D::~Particle3D()
{
	for (int i = (int)particle.size() - 1; i >= 0; i--)
	{
		delete particle[i];
		particle.erase(particle.begin() + i);
	}
}

void Particle3D::Init()
{}

void Particle3D::Update()
{
	for (size_t i = 0; i < particle.size(); i++)
	{
		//フレーム更新
		particle[i]->framNow++;
		//速度を足す
		particle[i]->position += particle[i]->velocity;
		particle[i]->velocity += particle[i]->accel;
		particle[i]->angle += particle[i]->anglePlas;
		float f = (float)particle[i]->framNow / particle[i]->framMax;
		if (f >= 1.0f)
		{
			delete particle[i];
			particle.erase(particle.begin() + i);
		}
	}
}

void Particle3D::Draw()
{
	for (auto p : particle)
	{
		if (p->graph.s_texNum == 0)
		{
			Object::NoShadowDraw(p->objData, p->psr,
				p->position, Vec3(1.0f, 1.0f, 1.0f), p->angle, Vec2(), p->graph);
		}
		else
		{
			Object::NoShadowDraw(p->objData, p->psr,
				p->position, Vec3(1.0f, 1.0f, 1.0f), p->angle, Vec2(), p->graph);
		}
	}
}

void Particle3D::Create(const ObjectData& objData, const Vec3& pos, const Vec3& velocity, const Vec3& accel, const Vec3& anglePlas, const int time, const TextureData& graph)
{
	particle.push_back(new ParticleData);
	size_t num = particle.size() - 1;
	particle[num]->objData = objData;
	particle[num]->position = pos;
	particle[num]->velocity = velocity;
	particle[num]->accel = accel;
	particle[num]->anglePlas = anglePlas;
	particle[num]->framMax = time;
	particle[num]->graph = graph;
}

Particle3D* Particle3D::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	Particle3D* particle = new Particle3D();
	return particle;
}
