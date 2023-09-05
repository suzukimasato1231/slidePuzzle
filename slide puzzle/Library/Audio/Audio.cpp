#include "Audio.h"
#include <cassert>

Audio::Audio()
{
}

void Audio::Init()
{
	HRESULT result;

	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2->CreateMasteringVoice(&masterVoice);


}
Audio *Audio::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Audio *audio = new Audio();
	if (audio == nullptr) {
		return nullptr;
	}
	audio->Init();
	// ������
	return audio;
}


SoundData Audio::SoundLoadWave(const char *filename)
{
	//�t�@�C���I�[�v��
	std::ifstream file;
	file.open(filename, std::ios_base::binary);

	assert(file.is_open());

	//2
	RiffHeader riff;
	file.read((char *)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}


	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	FormatChunk format = {};

	file.read((char *)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt", 4) != 0)
	{
		//assert(0);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char *)&format.fmt, format.chunk.size);


	ChunkHeader data;
	file.read((char *)&data, sizeof(data));
	if (strncmp(data.id, "JUNK", 4) == 0 || strncmp(data.id, "LIST", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);
		file.read((char *)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	char *pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE *>(pBuffer);
	soundData.BufferSize = data.size;


	return soundData;
}

void Audio::SoundUnload(SoundData *soundData)
{
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->BufferSize = 0;
	soundData->wfex = {};
}

void Audio::SoundSEPlayWave( const SoundData &soundData)
{
	HRESULT result;

	IXAudio2SourceVoice *pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.BufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// �g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);


	result = pSourceVoice->Start();
}

void Audio::SoundBGMPlayLoopWave(const SoundData &soundData, IXAudio2SourceVoice *pSourceVoice)
{
	HRESULT result;

	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.BufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	// �g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);


	result = pSourceVoice->Start();
}

void Audio::SoundStop( const SoundData &soundData,IXAudio2SourceVoice *pSourceVoice)
{
	HRESULT result;

	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_VOICE_STATE xa2state;
	pSourceVoice->GetState(&xa2state);


	result = pSourceVoice->Stop(0);
}

void Audio::SetVolume(float volume)
{
	masterVoice->SetVolume(volume);
}
