#include "LoadCSV.h"
#include <stdio.h>

int LoadCSV(int map[MAP_HEIGHT][MAP_WIDTH], const std::string &FilePath, int LoadStopNumber)
{
	if (map == nullptr)
	{//�Ȃ�������߂�
		return -1;
	}

	for (size_t y = 0; y < MAP_HEIGHT; y++)
	{
		for (size_t x = 0; x < MAP_WIDTH; x++)
		{
			// ������
			map[y][x] = 0;
		}
	}

	FILE *fileHandle;
	errno_t err;
	char string[256] = { 0 };

	err = fopen_s(&fileHandle, FilePath.c_str(), "r");
	if (err != 0)
	{
		return err;
	}

	bool isMinus = false;

	for (int y = 0; y < MAP_HEIGHT;)
	{
		bool end = false;
		fgets(string, 256, fileHandle);
		for (int x = 0, i = 0; x < MAP_WIDTH; i++)
		{
			if (string[i] == '\0')
			{
				// �ǂݍ��ݏI��
				fgets(string, 256, fileHandle);
				i = 0;
			}
			else if (string[i] == '\n')
			{
				// ���s
				y++;
				break;
			}
			else if (string[i] == ',')
			{
				// ���̐�����
				x++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{//������2���ȏ�̏���
				map[y][x] *= 10;

				if (isMinus == true)
				{
					map[y][x] -= string[i] - '0';
				}
				else
				{
					map[y][x] += string[i] - '0';
				}

				if (map[y][x] == LoadStopNumber)
				{
					// �q�b�g������A�ǂݍ��݂������I�ɏI������B
					end = true;
					break;
				}
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	return 0;
}

int LoadCSV1D(int *mapArray, const size_t &mapSize, const std::string& FilePath, int LoadStopNumber)
{
	for (size_t i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	FILE *fileHandle;
	errno_t err;
	char string[256] = { 0 };
	int index = 0;

	err = fopen_s(&fileHandle, FilePath.c_str(), "r");
	if (err != 0)
	{
		return err;
	}

	bool isMinus = false;

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		bool end = false;

		for (int i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == ',' || string[i] == '\n')
			{
				// ���̐�����
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}

				if (mapArray[index] == LoadStopNumber)
				{
					// �q�b�g������A�ǂݍ��݂������I�ɏI������B
					end = true;
					break;
				}
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	return 0;
}
