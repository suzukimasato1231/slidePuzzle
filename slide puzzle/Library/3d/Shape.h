#pragma once
#include"Object.h"
/// <summary>
/// �}�`���f��
/// </summary>
class Shape
{
public:
	Shape();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	static void Init(ID3D12Device* dev);
public://�}�`���f��
	/// <summary>
	/// �O�p��
	/// </summary>
	/// <param name="width">���̒���</param>
	/// <param name="height">�c�̒���</param>
	/// <returns></returns>
	static ObjectData CreateTriangularPyramid(const float width, const float height);
	/// <summary>
	/// �Z�`
	/// </summary>
	/// <param name="width">���̒���</param>
	/// <param name="height">�c�̒���</param>
	/// <returns></returns>
	static ObjectData CreateRect(const float width, const float height);
	/// <summary>
	/// �~
	/// </summary>
	/// <param name="r">���a</param>
	/// <param name="num">������</param>
	/// <returns></returns>
	static ObjectData CreateCircle(const float r, const int num);
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="width">���̒���</param>
	/// <param name="height">�c�̒���</param>
	/// <param name="depth">�[��</param>
	/// <returns></returns>
	static ObjectData CreateSquare(const float width, const float height, const float depth);
	/// <summary>
	/// �~��
	/// </summary>
	/// <param name="division">������</param>
	/// <param name="prizmHeight">����</param>
	/// <param name="radius">��ʂ̔��a</param>
	/// <returns></returns>
	static ObjectData CreateCylinder(int division, int prizmHeight, int radius);
public://OBJ�֘A
	/// <summary>
	/// OBJ�I�u�W�F�N�g�쐬
	/// </summary>
	/// <param name="filename">filename</param>
	/// <param name="smoothing"></param>
	/// <returns></returns>
	static ObjectData CreateOBJ(const std::string &filename, bool smoothing = false, const std::string &filePath = "");
	//�}�e���A���ǂݍ���
	static int LoadMaterial(const std::string& directoryPath, const std::string& filename, ObjectData& polygon);
private:
	/// <summary>
	/// �G�b�W�������f�[�^�̒ǉ�
	/// </summary>
	/// <param name="indexPosition">���W�C���f�b�N�X</param>
	/// <param name="indexVertex">���_�C���f�b�N�X</param>
	static void AddSmoothData(ObjectData& polygon, unsigned short indexPosition, unsigned short indexVertex);
	///<summary>
	///���������ꂽ���_�@���̌v�Z
	/// </summary>
	static void CalculateSmoothedVertexNormals(ObjectData& polygon);
	/// <summary>
	/// ���_�f�[�^�̐����擾
	/// </summary>
	/// <returns>���_�f�[�^�̐�</returns>
	static inline size_t GetVertexCount(ObjectData& polygon);
public:
	static ID3D12Device* dev;					//�f�o�C�X

	static void CreateModel(ObjectData& polygon);

	static void OBJCreateModel(ObjectData& polygon);
};