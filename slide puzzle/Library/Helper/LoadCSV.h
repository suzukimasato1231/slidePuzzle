#pragma once
#include<string>
#define MAP_WIDTH (20) //���̍ő喇��
#define MAP_HEIGHT (20)  //�c�̍ő喇��
#define MAP_ (100)  //�c�̍ő喇��
#include<Vec.h>
/// <summary>
/// CSV�t�@�C���̓ǂݍ���
/// </summary>
/// <param name="map"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
/// <param name="FilePath"> CSV�t�@�C���̃p�X </param>
/// <param name="LoadStopNumber"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
/// <returns> 0�ȊO�Ŏ��s�A0�Ő��� </returns>
int LoadCSV(int map[MAP_HEIGHT][MAP_WIDTH], const std::string& FilePath,const int width,const int height);

/// <summary>
/// CSV�t�@�C���̓ǂݍ���(1�����z��)
/// </summary>
/// <param name="mapArray"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
/// <param name="FilePath"> CSV�t�@�C���̃p�X </param>
/// <param name="LoadStopNumber"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
/// <returns> 0�ȊO�Ŏ��s�A0�Ő��� </returns>
int LoadCSV1D(int *mapArray, const size_t &mapSize, const std::string& FilePath, int LoadStopNumber = -1);

/// <summary>
/// �T�C�Y�𒲂ׂ�
/// </summary>
/// <param name="width">���T�C�Y�����</param>
/// <param name="height">�c�T�C�Y������</param>
void LoadSize(int &width,int &height, const std::string& FilePath);