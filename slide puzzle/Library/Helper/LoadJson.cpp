#include "LoadJson.h"
#include<fstream>
#include<cassert>
#include<json.hpp>
LevelData* LoadJson::Load(const std::string& fileName)
{
	//�A�����ăt���p�X�𓾂�
	const std::string fullpath = std::string("Resources/levels/") + fileName + ".json";

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"naem"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();
	//���������x���t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);


	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelData* levelData = new LevelData();

	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//��ނ��Ƃ̏���
		if (type.compare("MESH") == 0)
		{		
			//�v�f��ǉ�
			levelData->objects.emplace_back(LevelData::ObjectDataJSON{});
			LevelData::ObjectDataJSON& objectData = levelData->objects.back();

			objectData.type = MESHTYPE;

			if (object.contains("file_name"))
			{
				//�t�@�C����
				objectData.fileName = object["file_name"];
			}
			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& tranform = object["transform"];
			//���s�ړ�
			objectData.translation.x = (float)tranform["translation"][1];
			objectData.translation.y = (float)tranform["translation"][2];
			objectData.translation.z = (float)-tranform["translation"][0];
			//��]�p
			objectData.rotation.x = (float)-tranform["rotation"][1];
			objectData.rotation.y = (float)-tranform["rotation"][2];
			objectData.rotation.z = (float)tranform["rotation"][0];
			//�X�P�[�����O
			objectData.scaling.x = (float)tranform["scaling"][1];
			objectData.scaling.y = (float)tranform["scaling"][2];
			objectData.scaling.z = (float)tranform["scaling"][0];
		}
	}
	return levelData;
}
