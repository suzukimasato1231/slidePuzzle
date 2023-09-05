#include"Shape.h"
#include<fstream>
#include<sstream>
using namespace std;
ID3D12Device* Shape::dev = nullptr;
Shape::Shape()
{}

void Shape::Init(ID3D12Device* dev)
{
	Shape::dev = dev;
}

ObjectData Shape::CreateTriangularPyramid(const float width, const float height)
{
	ObjectData polygon;
	const int DIV = 3;
	//三角錐
	for (int i = 0; i < DIV; i++)
	{
		polygon.vertices.push_back(new Vertex);
		polygon.vertices[polygon.vertices.size() - 1]->pos = { width * sinf(2 * XM_PI / DIV * i) ,width * cosf(2 * XM_PI / DIV * i),0 };
	}
	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { 0,0,0 };
	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { 0,0,-height };

	for (int i = 0; i < polygon.vertices.size(); i++)
	{
		polygon.vertices[i]->normal = {};

	}
	polygon.vertices[0]->uv = { 0.0f,0.0f }, polygon.vertices[1]->uv = { 1.0f,0.0f };
	polygon.vertices[2]->uv = { 0.0f,1.0f }, polygon.vertices[3]->uv = { 1.0f,1.0f };
	polygon.vertices[4]->uv = { 1.0f,1.0f };
	//インデックス
	//底面
	polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(0)), polygon.indices.push_back(new unsigned short(3));
	polygon.indices.push_back(new unsigned short(2)), polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(3));
	polygon.indices.push_back(new unsigned short(0)), polygon.indices.push_back(new unsigned short(2)), polygon.indices.push_back(new unsigned short(3));
	//側面
	polygon.indices.push_back(new unsigned short(0)), polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(4));
	polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(2)), polygon.indices.push_back(new unsigned short(4));
	polygon.indices.push_back(new unsigned short(2)), polygon.indices.push_back(new unsigned short(0)), polygon.indices.push_back(new unsigned short(4));

	CreateModel(polygon);
	polygon.indicesNum = (UINT)polygon.indices.size();
	return polygon;
}

ObjectData Shape::CreateRect(const float width, const float height)
{
	ObjectData polygon;
	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2, -height / 2, 0.0f };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2, height / 2, 0.0f };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,0.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2, -height / 2,  0.0f };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2, height / 2, 0.0f };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,0.0f };


	for (int i = 0; i < polygon.vertices.size(); i++)
	{
		polygon.vertices[i]->normal = {};
	}

	polygon.indices.push_back(new unsigned short(0)), polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(2));

	polygon.indices.push_back(new unsigned short(2)), polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(3));

	CreateModel(polygon);
	polygon.indicesNum = (UINT)polygon.indices.size();
	return polygon;
}

ObjectData Shape::CreateCircle(const float r, const int num)
{
	ObjectData polygon;
	for (int i = 0; i < num; i++)
	{
		polygon.vertices.push_back(new Vertex);
		polygon.vertices[polygon.vertices.size() - 1]->pos.x = r * sinf(6.28f / num * i);
		polygon.vertices[polygon.vertices.size() - 1]->pos.y = r * cosf(6.28f / num * i);
		polygon.vertices[polygon.vertices.size() - 1]->pos.z = 0;
		polygon.vertices[polygon.vertices.size() - 1]->uv = { 0,0 };
	}

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { 0,0,0 };

	for (int i = 0; i < num; i++)
	{
		polygon.indices.push_back(new unsigned short(i));
		polygon.indices.push_back(new unsigned short(i + 1));
		polygon.indices.push_back(new unsigned short(num));
	}
	*polygon.indices[polygon.indices.size() - 2] = 0;

	CreateModel(polygon);
	polygon.indicesNum = (UINT)polygon.indices.size();
	return polygon;
}

ObjectData Shape::CreateSquare(const float width, const float height, const float depth)
{
	ObjectData polygon;
	//前
	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,-height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,0.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,-height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,0.0f };

	//後ろ
	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,-height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,0.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,-height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,0.0f };

	//左
	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,-height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,-height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,0.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,0.0f };


	//右
	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,-height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,-height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,0.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,0.0f };


	//上
	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,-height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,-height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,0.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,-height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,-height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,0.0f };

	//下
	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { -width / 2,height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 0.0f,0.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,height / 2,depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,1.0f };

	polygon.vertices.push_back(new Vertex);
	polygon.vertices[polygon.vertices.size() - 1]->pos = { width / 2,height / 2,-depth / 2 };
	polygon.vertices[polygon.vertices.size() - 1]->uv = { 1.0f,0.0f };
	//前
	polygon.indices.push_back(new unsigned short(0)), polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(2));
	polygon.indices.push_back(new unsigned short(2)), polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(3));
	//後ろ
	polygon.indices.push_back(new unsigned short(5)), polygon.indices.push_back(new unsigned short(4)), polygon.indices.push_back(new unsigned short(6));
	polygon.indices.push_back(new unsigned short(5)), polygon.indices.push_back(new unsigned short(6)), polygon.indices.push_back(new unsigned short(7));
	//左
	polygon.indices.push_back(new unsigned short(8)), polygon.indices.push_back(new unsigned short(9)), polygon.indices.push_back(new unsigned short(10));
	polygon.indices.push_back(new unsigned short(10)), polygon.indices.push_back(new unsigned short(9)), polygon.indices.push_back(new unsigned short(11));
	//右
	polygon.indices.push_back(new unsigned short(13)), polygon.indices.push_back(new unsigned short(12)), polygon.indices.push_back(new unsigned short(14));
	polygon.indices.push_back(new unsigned short(13)), polygon.indices.push_back(new unsigned short(14)), polygon.indices.push_back(new unsigned short(15));
	//上
	polygon.indices.push_back(new unsigned short(16)), polygon.indices.push_back(new unsigned short(17)), polygon.indices.push_back(new unsigned short(18));
	polygon.indices.push_back(new unsigned short(18)), polygon.indices.push_back(new unsigned short(17)), polygon.indices.push_back(new unsigned short(19));
	//下
	polygon.indices.push_back(new unsigned short(21)), polygon.indices.push_back(new unsigned short(20)), polygon.indices.push_back(new unsigned short(22));
	polygon.indices.push_back(new unsigned short(21)), polygon.indices.push_back(new unsigned short(22)), polygon.indices.push_back(new unsigned short(23));
	CreateModel(polygon);
	polygon.indicesNum = (UINT)polygon.indices.size();
	return polygon;
}

ObjectData Shape::CreateCylinder(int division, int prizmHeight, int radius)
{
	ObjectData polygon;

	HRESULT result = S_FALSE;

	std::vector<Vertex> realVertices;
	// 頂点座標の計算（重複あり）
	{
		realVertices.resize((division + 1) * 2);
		int index = 0;
		float zValue;

		// 底面
		zValue = prizmHeight / 2.0f;
		for (int i = 0; i < division; i++)
		{
			Vec3 vertex;
			vertex.x = radius * sinf(XM_2PI / division * i);
			vertex.y = radius * cosf(XM_2PI / division * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		realVertices[index++].pos = Vec3(0, 0, zValue);	// 底面の中心点
		// 天面
		zValue = -prizmHeight / 2.0f;
		for (int i = 0; i < division; i++)
		{
			Vec3 vertex;
			vertex.x = radius * sinf(XM_2PI / division * i);
			vertex.y = radius * cosf(XM_2PI / division * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		realVertices[index++].pos = Vec3(0, 0, zValue);	// 天面の中心点
	}
	// 頂点座標の計算（重複なし）
	{
		// 底面
		for (int i = 0; i < division; i++)
		{
			unsigned short index0 = i + 1;
			unsigned short index1 = i;
			unsigned short index2 = division;
			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index0].pos;
			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index1].pos;
			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index2].pos; // 底面の中心点
		}
		// 底面の最後の三角形の1番目のインデックスを0に書き換え
		polygon.vertices[polygon.vertices.size() - 3]->pos = realVertices[0].pos;

		int topStart = division + 1;
		// 天面
		for (int i = 0; i < division; i++)
		{
			unsigned short index0 = topStart + i;
			unsigned short index1 = topStart + i + 1;
			unsigned short index2 = topStart + division;
			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index0].pos;
			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index1].pos;
			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index2].pos; // 天面の中心点
		}
		// 天面の最後の三角形の1番目のインデックスを0に書き換え
		polygon.vertices[polygon.vertices.size() - 2]->pos = realVertices[topStart].pos;

		// 側面
		for (int i = 0; i < division; i++)
		{
			unsigned short index0 = i + 1;
			unsigned short index1 = topStart + i + 1;
			unsigned short index2 = i;
			unsigned short index3 = topStart + i;

			if (i == division - 1)
			{
				index0 = 0;
				index1 = topStart;
			}
			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index0].pos;

			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index1].pos;

			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index2].pos;

			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index2].pos;

			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index1].pos;

			polygon.vertices.push_back(new Vertex);
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index3].pos;
		}
	}
	int planeCount = division * 2 + division * 2;		// 面の数
	planeCount = planeCount * 3;
	// 頂点インデックスの設定
	{
		for (int i = 0; i < planeCount; i++)
		{
			polygon.indices.push_back(new unsigned short(i));
		}
	}
	CreateModel(polygon);

	polygon.indicesNum = (UINT)polygon.indices.size();
	return polygon;
}

ObjectData Shape::CreateOBJ(const std::string &filename, bool smoothing, const std::string &filePath)
{
	ObjectData polygon;

	int index = 0;//インデックスの数調整
	//ファイルストリーム
	std::ifstream file;
	//.objファイルを開く
	std::string directoryPath = "Resources/" + filePath + filename + "/";
	file.open(directoryPath + filename + ".obj");
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}
	vector<Vec3>positions;//頂点座標
	vector<Vec3>normals;//法線ベクトル
	vector<Vec2>texcoords;//テクスチャUV
	//１行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			//マテリアル読み込み
			LoadMaterial(directoryPath, filename, polygon);
		}

		//先頭文字がvなら頂点座標
		if (key == "v")
		{
			//X,Y,Z座標読み込み
			Vec3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}
		//先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			//U,V成分読み込み
			Vec2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		//先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			//X,Y,Z成分読み込み
			Vec3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		//先頭文字列がfならポリゴン（三角形）
		if (key == "f")
		{
			int faceIndexCount = 0;
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//頂点インデックス１個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexNormal;

				//頂点データの追加
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				polygon.vertices.push_back(new Vertex);
				polygon.vertices[polygon.vertices.size() - 1]->pos = vertex.pos;
				polygon.vertices[polygon.vertices.size() - 1]->normal = vertex.normal;
				polygon.vertices[polygon.vertices.size() - 1]->uv = vertex.uv;
				//エッジ円滑化用のデータを追加
				if (smoothing)
				{
					//vキー(座標データ)の番号と、全て合成した頂点のインデックスをセットで登録する
					AddSmoothData(polygon, indexPosition, (unsigned short)GetVertexCount(polygon) - 1);
				}
				// インデックスデータの追加
				if (faceIndexCount >= 3) {
					// 四角形ポリゴンの4点目なので、
					// 四角形の0,1,2,3の内 2,3,0で三角形を構築する
					polygon.indices.push_back(new unsigned short(static_cast<unsigned short>(polygon.indices.size() - 1 - index)));
					polygon.indices.push_back(new unsigned short(static_cast<unsigned short>(polygon.indices.size() - 1 - index)));
					polygon.indices.push_back(new unsigned short(static_cast<unsigned short>(polygon.indices.size() - 5 - index))
					);
					index += 2;
				}
				else
				{
					polygon.indices.push_back(new unsigned short(static_cast<unsigned short>(polygon.indices.size() - index)));
				}

				faceIndexCount++;

			}
		}
	}
	//ファイルを閉じる
	file.close();
	//頂点法線の平均によるエッジを円滑化
	if (smoothing)
	{
		CalculateSmoothedVertexNormals(polygon);
	}
	OBJCreateModel(polygon);

	polygon.indicesNum = (UINT)polygon.indices.size();

	return polygon;
}



int Shape::LoadMaterial(const std::string& directoryPath, const std::string& filename, ObjectData& polygon)
{
	int texNumber = 0;
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}
	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');
		//先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin());//先頭の文字を削除
		}
		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			//マテリアル名読み込み
			line_stream >> polygon.material.name;
		}
		//先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> polygon.material.ambient.x;
			line_stream >> polygon.material.ambient.y;
			line_stream >> polygon.material.ambient.z;
		}
		//先頭文字列がKdならディフューズ色
		if (key == "Kd")
		{
			line_stream >> polygon.material.diffuse.x;
			line_stream >> polygon.material.diffuse.y;
			line_stream >> polygon.material.diffuse.z;
		}
		//先頭文字列がKsならスペキュラー色
		if (key == "Ks")
		{
			line_stream >> polygon.material.specular.x;
			line_stream >> polygon.material.specular.y;
			line_stream >> polygon.material.specular.z;
		}
		//先頭文字列がmap_Kdならテクスチャ名
		if (key == "map_Kd")
		{
			//テクスチャのファイル名読み込み
			line_stream >> polygon.material.textureFilename;
			//テクスチャ読み込み
			polygon.OBJTexture = Texture::Get()->OBJLoadTexture(directoryPath, polygon.material.textureFilename);
		}
	}
	//ファイルを閉じる
	file.close();
	return  0;
}

void Shape::CreateModel(ObjectData& polygon)
{
	HRESULT result = S_FALSE;
	for (size_t i = 0; i < polygon.indices.size() / 3; i++)
	{//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short indices0 = *polygon.indices[i * 3 + 0];
		unsigned short indices1 = *polygon.indices[i * 3 + 1];
		unsigned short indices2 = *polygon.indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&polygon.vertices[indices0]->pos);
		XMVECTOR p1 = XMLoadFloat3(&polygon.vertices[indices1]->pos);
		XMVECTOR p2 = XMLoadFloat3(&polygon.vertices[indices2]->pos);
		//p0→p1ベクトル、p0→p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化（長さを１にする）
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&polygon.vertices[indices0]->normal, normal);
		XMStoreFloat3(&polygon.vertices[indices1]->normal, normal);
		XMStoreFloat3(&polygon.vertices[indices2]->normal, normal);
	}

	//頂点データ全体のサイズ＝頂点データ一つ分のサイズ＊頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * polygon.vertices.size());

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * polygon.indices.size());

	////頂点バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&polygon.vertBuff)
	);

	////インデックスバッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&polygon.indexBuff));

	Vertex* vertMap = nullptr;
	//GPU上のバッファに対応した仮想メモリを取得
	result = polygon.vertBuff->Map(0, nullptr, (void**)&vertMap);

	//全頂点に対して
	for (size_t i = 0; i < polygon.vertices.size(); i++)
	{
		vertMap[i] = *polygon.vertices[i];//座標コピー
	}

	//マップを解除
	polygon.vertBuff->Unmap(0, nullptr);

	//GPU上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;
	result = polygon.indexBuff->Map(0, nullptr, (void**)&indexMap);

	//全インデックスに対して
	for (size_t i = 0; i < polygon.indices.size(); i++)
	{
		indexMap[i] = *polygon.indices[i];//インデックスをコピー
	}
	//繋がりを解除
	polygon.indexBuff->Unmap(0, nullptr);

	//頂点バッファビューの作成
	polygon.vbView.BufferLocation =
		polygon.vertBuff->GetGPUVirtualAddress();
	polygon.vbView.SizeInBytes = sizeVB;
	polygon.vbView.StrideInBytes = sizeof(Vertex);

	//インデックスバッファビューの作成
	polygon.ibView.BufferLocation = polygon.indexBuff->GetGPUVirtualAddress();
	polygon.ibView.Format = DXGI_FORMAT_R16_UINT;
	polygon.ibView.SizeInBytes = sizeIB;
}

void Shape::OBJCreateModel(ObjectData& polygon)
{
	HRESULT result;
	//頂点データ全体のサイズ＝頂点データ一つ分のサイズ＊頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * polygon.vertices.size());

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * polygon.indices.size());

	////頂点バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&polygon.vertBuff)
	);

	////インデックスバッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&polygon.indexBuff));

	Vertex* vertMap = nullptr;
	//GPU上のバッファに対応した仮想メモリを取得
	result = polygon.vertBuff->Map(0, nullptr, (void**)&vertMap);

	//全頂点に対して
	for (size_t i = 0; i < polygon.vertices.size(); i++)
	{
		vertMap[i] = *polygon.vertices[i];//座標コピー
	}

	//マップを解除
	polygon.vertBuff->Unmap(0, nullptr);

	//GPU上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;
	result = polygon.indexBuff->Map(0, nullptr, (void**)&indexMap);

	//全インデックスに対して
	for (size_t i = 0; i < polygon.indices.size(); i++)
	{
		indexMap[i] = *polygon.indices[i];//インデックスをコピー
	}
	//繋がりを解除
	polygon.indexBuff->Unmap(0, nullptr);

	//頂点バッファビューの作成
	polygon.vbView.BufferLocation =
		polygon.vertBuff->GetGPUVirtualAddress();
	polygon.vbView.SizeInBytes = sizeVB;
	polygon.vbView.StrideInBytes = sizeof(Vertex);

	//インデックスバッファビューの作成
	polygon.ibView.BufferLocation = polygon.indexBuff->GetGPUVirtualAddress();
	polygon.ibView.Format = DXGI_FORMAT_R16_UINT;
	polygon.ibView.SizeInBytes = sizeIB;
}

void Shape::AddSmoothData(ObjectData& polygon, unsigned short indexPosition, unsigned short indexVertex)
{
	polygon.smoothData[indexPosition].emplace_back(indexVertex);
}

inline size_t Shape::GetVertexCount(ObjectData& polygon)
{
	return polygon.vertices.size();
}

void Shape::CalculateSmoothedVertexNormals(ObjectData& polygon)
{
	auto itr = polygon.smoothData.begin();
	for (; itr != polygon.smoothData.end(); ++itr)
	{
		//各面用の共通頂点コレクション
		std::vector<unsigned short>& v = itr->second;
		//全頂点の法線を平均する
		XMVECTOR normal = {};
		for (unsigned short index : v)
		{
			normal += XMVectorSet(polygon.vertices[index]->normal.x, polygon.vertices[index]->normal.y, polygon.vertices[index]->normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//共通法線を使用する全ての頂点データに書き込む
		for (unsigned short index : v)
		{
			polygon.vertices[index]->normal = { normal.m128_f32[0],normal.m128_f32[1],normal.m128_f32[2] };
		}
	}
}