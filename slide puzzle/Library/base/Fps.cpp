#include "FPS.h"


void FPS::Start()
{
	// ���C�����[�v�ɓ���O�ɐ��x���擾���Ă���
	if (QueryPerformanceFrequency(&m_timeFreq) == FALSE) { // ���̊֐���0(FALSE)���A�鎞�͖��Ή�
		// ��������QueryPerformanceFrequency���g���Ȃ��l��(�Â�)PC�ł͂ǂ����F�X�L�c�C���낤��
		return; // �{���͂���ȋA������Ă͍s���Ȃ�(�㑱�̉���������Ă΂�Ȃ�)
	}
	// 1�x�擾���Ă���(����v�Z�p)
	QueryPerformanceCounter(&m_timeStart);
}

void FPS::run()
{
	// ���̎��Ԃ��擾
	QueryPerformanceCounter(&m_timeEnd);
	// (���̎��� - �O�t���[���̎���) / ���g�� = �o�ߎ���(�b�P��)
	m_frameTime = static_cast<float>(m_timeEnd.QuadPart - m_timeStart.QuadPart) / static_cast<float>(m_timeFreq.QuadPart);

	if (m_frameTime < c_MIN_FREAM_TIME) { // ���Ԃɗ]�T������
		// �~���b�ɕϊ�
		DWORD sleepTime = static_cast<DWORD>((c_MIN_FREAM_TIME - m_frameTime) * 1000);

		timeBeginPeriod(1); // ����\���グ��(�������Ȃ���Sleep�̐��x�̓K�^�K�^)
		Sleep(sleepTime);   // �Q��
		timeEndPeriod(1);   // �߂�

		// ���T�Ɏ����z��(�������Ȃ���fps���ςɂȂ�?)
		return;
	}

	if (m_frameTime > 0.0) { // �o�ߎ��Ԃ�0���傫��(�������Ȃ��Ɖ��̌v�Z�Ń[�����Z�ɂȂ�Ǝv���)
		m_fps = (m_fps * 0.99f) + (0.01f / m_frameTime); // ����fps���v�Z
	}

	m_timeStart = m_timeEnd; // ����ւ�

}

FPS* FPS::Get()
{
	static FPS ins;

	return &ins;
}
