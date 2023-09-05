#pragma once
#include<string>
#include<vector>
#include<Vec.h>
enum TYPE
{
	MESHTYPE,
	CAMERATYPE,
};

struct LevelData
{
	struct ObjectDataJSON
	{
		//オブジェクトのファイル名
		std::string fileName;
		Vec3 translation;
		Vec3 rotation;
		Vec3 scaling;
		int type = 0;
		int objectType = 0;
	};
	//オブジェクトのコンテナ
	std::vector<ObjectDataJSON>objects;
};

class LoadJson
{
public:
	static LevelData* Load(const std::string& fileName);
};

