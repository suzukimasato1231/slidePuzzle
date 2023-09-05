#include "LoadJson.h"
#include<fstream>
#include<cassert>
#include<json.hpp>
LevelData* LoadJson::Load(const std::string& fileName)
{
	//連結してフルパスを得る
	const std::string fullpath = std::string("Resources/levels/") + fileName + ".json";

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"naem"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルファイル化チェック
	assert(name.compare("scene") == 0);


	//レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		if (type.compare("MESH") == 0)
		{		
			//要素を追加
			levelData->objects.emplace_back(LevelData::ObjectDataJSON{});
			LevelData::ObjectDataJSON& objectData = levelData->objects.back();

			objectData.type = MESHTYPE;

			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName = object["file_name"];
			}
			//トランスフォームのパラメータ読み込み
			nlohmann::json& tranform = object["transform"];
			//平行移動
			objectData.translation.x = (float)tranform["translation"][1];
			objectData.translation.y = (float)tranform["translation"][2];
			objectData.translation.z = (float)-tranform["translation"][0];
			//回転角
			objectData.rotation.x = (float)-tranform["rotation"][1];
			objectData.rotation.y = (float)-tranform["rotation"][2];
			objectData.rotation.z = (float)tranform["rotation"][0];
			//スケーリング
			objectData.scaling.x = (float)tranform["scaling"][1];
			objectData.scaling.y = (float)tranform["scaling"][2];
			objectData.scaling.z = (float)tranform["scaling"][0];
		}
	}
	return levelData;
}
