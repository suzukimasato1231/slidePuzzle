#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
#include <cassert>
#include<d3dx12.h>
#include"Singleton.h"
//�`�����N�w�b�_
struct ChunkHeader
{
	char id[4];
	int32_t size;//�`�����N�T�C�Y
};

struct RiffHeader
{
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk
{
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct SoundData
{
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	unsigned int BufferSize;
};
/// <summary>
/// �T�E���h�N���X
/// </summary>
class Audio : public Singleton<Audio>
{
private:
	friend Singleton<Audio>;
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<IXAudio2>xAudio2;
	IXAudio2MasteringVoice *masterVoice;
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Audio();
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// ���쐬
	/// </summary>
	/// <returns></returns>
	static Audio *Create();

	//WAVE�ǂݍ���
	static SoundData SoundLoadWave(const char *firename);
	//�������폜
	static void SoundUnload(SoundData *soundData);
	//1��Đ�
	void Audio::SoundSEPlayWave( const SoundData &soundData);
	//���[�v�Đ�
	void Audio::SoundBGMPlayLoopWave( const SoundData &soundData, IXAudio2SourceVoice *pSourceVoice);

	void Audio::SoundStop( const SoundData &soundData, IXAudio2SourceVoice *pSourceVoice);
	//���ʒ���
	void Audio::SetVolume(float volume);

	IXAudio2SourceVoice *BGM = nullptr;
};

