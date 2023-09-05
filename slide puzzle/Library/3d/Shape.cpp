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
	//�O�p��
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
	//�C���f�b�N�X
	//���
	polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(0)), polygon.indices.push_back(new unsigned short(3));
	polygon.indices.push_back(new unsigned short(2)), polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(3));
	polygon.indices.push_back(new unsigned short(0)), polygon.indices.push_back(new unsigned short(2)), polygon.indices.push_back(new unsigned short(3));
	//����
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
	//�O
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

	//���
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

	//��
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


	//�E
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


	//��
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

	//��
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
	//�O
	polygon.indices.push_back(new unsigned short(0)), polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(2));
	polygon.indices.push_back(new unsigned short(2)), polygon.indices.push_back(new unsigned short(1)), polygon.indices.push_back(new unsigned short(3));
	//���
	polygon.indices.push_back(new unsigned short(5)), polygon.indices.push_back(new unsigned short(4)), polygon.indices.push_back(new unsigned short(6));
	polygon.indices.push_back(new unsigned short(5)), polygon.indices.push_back(new unsigned short(6)), polygon.indices.push_back(new unsigned short(7));
	//��
	polygon.indices.push_back(new unsigned short(8)), polygon.indices.push_back(new unsigned short(9)), polygon.indices.push_back(new unsigned short(10));
	polygon.indices.push_back(new unsigned short(10)), polygon.indices.push_back(new unsigned short(9)), polygon.indices.push_back(new unsigned short(11));
	//�E
	polygon.indices.push_back(new unsigned short(13)), polygon.indices.push_back(new unsigned short(12)), polygon.indices.push_back(new unsigned short(14));
	polygon.indices.push_back(new unsigned short(13)), polygon.indices.push_back(new unsigned short(14)), polygon.indices.push_back(new unsigned short(15));
	//��
	polygon.indices.push_back(new unsigned short(16)), polygon.indices.push_back(new unsigned short(17)), polygon.indices.push_back(new unsigned short(18));
	polygon.indices.push_back(new unsigned short(18)), polygon.indices.push_back(new unsigned short(17)), polygon.indices.push_back(new unsigned short(19));
	//��
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
	// ���_���W�̌v�Z�i�d������j
	{
		realVertices.resize((division + 1) * 2);
		int index = 0;
		float zValue;

		// ���
		zValue = prizmHeight / 2.0f;
		for (int i = 0; i < division; i++)
		{
			Vec3 vertex;
			vertex.x = radius * sinf(XM_2PI / division * i);
			vertex.y = radius * cosf(XM_2PI / division * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		realVertices[index++].pos = Vec3(0, 0, zValue);	// ��ʂ̒��S�_
		// �V��
		zValue = -prizmHeight / 2.0f;
		for (int i = 0; i < division; i++)
		{
			Vec3 vertex;
			vertex.x = radius * sinf(XM_2PI / division * i);
			vertex.y = radius * cosf(XM_2PI / division * i);
			vertex.z = zValue;
			realVertices[index++].pos = vertex;
		}
		realVertices[index++].pos = Vec3(0, 0, zValue);	// �V�ʂ̒��S�_
	}
	// ���_���W�̌v�Z�i�d���Ȃ��j
	{
		// ���
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
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index2].pos; // ��ʂ̒��S�_
		}
		// ��ʂ̍Ō�̎O�p�`��1�Ԗڂ̃C���f�b�N�X��0�ɏ�������
		polygon.vertices[polygon.vertices.size() - 3]->pos = realVertices[0].pos;

		int topStart = division + 1;
		// �V��
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
			polygon.vertices[polygon.vertices.size() - 1]->pos = realVertices[index2].pos; // �V�ʂ̒��S�_
		}
		// �V�ʂ̍Ō�̎O�p�`��1�Ԗڂ̃C���f�b�N�X��0�ɏ�������
		polygon.vertices[polygon.vertices.size() - 2]->pos = realVertices[topStart].pos;

		// ����
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
	int planeCount = division * 2 + division * 2;		// �ʂ̐�
	planeCount = planeCount * 3;
	// ���_�C���f�b�N�X�̐ݒ�
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

	int index = 0;//�C���f�b�N�X�̐�����
	//�t�@�C���X�g���[��
	std::ifstream file;
	//.obj�t�@�C�����J��
	std::string directoryPath = "Resources/" + filePath + filename + "/";
	file.open(directoryPath + filename + ".obj");
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}
	vector<Vec3>positions;//���_���W
	vector<Vec3>normals;//�@���x�N�g��
	vector<Vec2>texcoords;//�e�N�X�`��UV
	//�P�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{
			//�}�e���A���̃t�@�C�����ǂݍ���
			string filename;
			line_stream >> filename;
			//�}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename, polygon);
		}

		//�擪������v�Ȃ璸�_���W
		if (key == "v")
		{
			//X,Y,Z���W�ǂݍ���
			Vec3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}
		//�擪������vt�Ȃ�e�N�X�`��
		if (key == "vt")
		{
			//U,V�����ǂݍ���
			Vec2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		//�擪������vn�Ȃ�@���x�N�g��
		if (key == "vn")
		{
			//X,Y,Z�����ǂݍ���
			Vec3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		//�擪������f�Ȃ�|���S���i�O�p�`�j
		if (key == "f")
		{
			int faceIndexCount = 0;
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//���_�C���f�b�N�X�P���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);//�X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);//�X���b�V�����΂�
				index_stream >> indexNormal;

				//���_�f�[�^�̒ǉ�
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				polygon.vertices.push_back(new Vertex);
				polygon.vertices[polygon.vertices.size() - 1]->pos = vertex.pos;
				polygon.vertices[polygon.vertices.size() - 1]->normal = vertex.normal;
				polygon.vertices[polygon.vertices.size() - 1]->uv = vertex.uv;
				//�G�b�W�~�����p�̃f�[�^��ǉ�
				if (smoothing)
				{
					//v�L�[(���W�f�[�^)�̔ԍ��ƁA�S�č����������_�̃C���f�b�N�X���Z�b�g�œo�^����
					AddSmoothData(polygon, indexPosition, (unsigned short)GetVertexCount(polygon) - 1);
				}
				// �C���f�b�N�X�f�[�^�̒ǉ�
				if (faceIndexCount >= 3) {
					// �l�p�`�|���S����4�_�ڂȂ̂ŁA
					// �l�p�`��0,1,2,3�̓� 2,3,0�ŎO�p�`���\�z����
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
	//�t�@�C�������
	file.close();
	//���_�@���̕��ςɂ��G�b�W���~����
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
	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}
	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);
		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');
		//�擪�̃^�u�����͖�������
		if (key[0] == '\t')
		{
			key.erase(key.begin());//�擪�̕������폜
		}
		//�擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl")
		{
			//�}�e���A�����ǂݍ���
			line_stream >> polygon.material.name;
		}
		//�擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			line_stream >> polygon.material.ambient.x;
			line_stream >> polygon.material.ambient.y;
			line_stream >> polygon.material.ambient.z;
		}
		//�擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd")
		{
			line_stream >> polygon.material.diffuse.x;
			line_stream >> polygon.material.diffuse.y;
			line_stream >> polygon.material.diffuse.z;
		}
		//�擪������Ks�Ȃ�X�y�L�����[�F
		if (key == "Ks")
		{
			line_stream >> polygon.material.specular.x;
			line_stream >> polygon.material.specular.y;
			line_stream >> polygon.material.specular.z;
		}
		//�擪������map_Kd�Ȃ�e�N�X�`����
		if (key == "map_Kd")
		{
			//�e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> polygon.material.textureFilename;
			//�e�N�X�`���ǂݍ���
			polygon.OBJTexture = Texture::Get()->OBJLoadTexture(directoryPath, polygon.material.textureFilename);
		}
	}
	//�t�@�C�������
	file.close();
	return  0;
}

void Shape::CreateModel(ObjectData& polygon)
{
	HRESULT result = S_FALSE;
	for (size_t i = 0; i < polygon.indices.size() / 3; i++)
	{//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short indices0 = *polygon.indices[i * 3 + 0];
		unsigned short indices1 = *polygon.indices[i * 3 + 1];
		unsigned short indices2 = *polygon.indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&polygon.vertices[indices0]->pos);
		XMVECTOR p1 = XMLoadFloat3(&polygon.vertices[indices1]->pos);
		XMVECTOR p2 = XMLoadFloat3(&polygon.vertices[indices2]->pos);
		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K���i�������P�ɂ���j
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&polygon.vertices[indices0]->normal, normal);
		XMStoreFloat3(&polygon.vertices[indices1]->normal, normal);
		XMStoreFloat3(&polygon.vertices[indices2]->normal, normal);
	}

	//���_�f�[�^�S�̂̃T�C�Y�����_�f�[�^����̃T�C�Y�����_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * polygon.vertices.size());

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * polygon.indices.size());

	////���_�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&polygon.vertBuff)
	);

	////�C���f�b�N�X�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&polygon.indexBuff));

	Vertex* vertMap = nullptr;
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	result = polygon.vertBuff->Map(0, nullptr, (void**)&vertMap);

	//�S���_�ɑ΂���
	for (size_t i = 0; i < polygon.vertices.size(); i++)
	{
		vertMap[i] = *polygon.vertices[i];//���W�R�s�[
	}

	//�}�b�v������
	polygon.vertBuff->Unmap(0, nullptr);

	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	unsigned short* indexMap = nullptr;
	result = polygon.indexBuff->Map(0, nullptr, (void**)&indexMap);

	//�S�C���f�b�N�X�ɑ΂���
	for (size_t i = 0; i < polygon.indices.size(); i++)
	{
		indexMap[i] = *polygon.indices[i];//�C���f�b�N�X���R�s�[
	}
	//�q���������
	polygon.indexBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	polygon.vbView.BufferLocation =
		polygon.vertBuff->GetGPUVirtualAddress();
	polygon.vbView.SizeInBytes = sizeVB;
	polygon.vbView.StrideInBytes = sizeof(Vertex);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	polygon.ibView.BufferLocation = polygon.indexBuff->GetGPUVirtualAddress();
	polygon.ibView.Format = DXGI_FORMAT_R16_UINT;
	polygon.ibView.SizeInBytes = sizeIB;
}

void Shape::OBJCreateModel(ObjectData& polygon)
{
	HRESULT result;
	//���_�f�[�^�S�̂̃T�C�Y�����_�f�[�^����̃T�C�Y�����_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * polygon.vertices.size());

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * polygon.indices.size());

	////���_�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&polygon.vertBuff)
	);

	////�C���f�b�N�X�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&polygon.indexBuff));

	Vertex* vertMap = nullptr;
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	result = polygon.vertBuff->Map(0, nullptr, (void**)&vertMap);

	//�S���_�ɑ΂���
	for (size_t i = 0; i < polygon.vertices.size(); i++)
	{
		vertMap[i] = *polygon.vertices[i];//���W�R�s�[
	}

	//�}�b�v������
	polygon.vertBuff->Unmap(0, nullptr);

	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	unsigned short* indexMap = nullptr;
	result = polygon.indexBuff->Map(0, nullptr, (void**)&indexMap);

	//�S�C���f�b�N�X�ɑ΂���
	for (size_t i = 0; i < polygon.indices.size(); i++)
	{
		indexMap[i] = *polygon.indices[i];//�C���f�b�N�X���R�s�[
	}
	//�q���������
	polygon.indexBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	polygon.vbView.BufferLocation =
		polygon.vertBuff->GetGPUVirtualAddress();
	polygon.vbView.SizeInBytes = sizeVB;
	polygon.vbView.StrideInBytes = sizeof(Vertex);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
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
		//�e�ʗp�̋��ʒ��_�R���N�V����
		std::vector<unsigned short>& v = itr->second;
		//�S���_�̖@���𕽋ς���
		XMVECTOR normal = {};
		for (unsigned short index : v)
		{
			normal += XMVectorSet(polygon.vertices[index]->normal.x, polygon.vertices[index]->normal.y, polygon.vertices[index]->normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//���ʖ@�����g�p����S�Ă̒��_�f�[�^�ɏ�������
		for (unsigned short index : v)
		{
			polygon.vertices[index]->normal = { normal.m128_f32[0],normal.m128_f32[1],normal.m128_f32[2] };
		}
	}
}