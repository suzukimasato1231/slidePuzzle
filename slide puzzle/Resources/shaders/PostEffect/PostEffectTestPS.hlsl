
#include "PostEffectTest.hlsli"

Texture2D<float4> tex : register(t0);//0�ԃX���b�g�Ɏw�肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);//�O�ԃX���b�g�Ɏw�肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//U���W�̕ω��ʂ�ϐ��ɓ����
	float U = 1.0f / 1200.0f;
	//V���W�̕ω��ʂ�ϐ��ɓ����
	float V = 1.0f / 720.0f;
	//������
	float4 totalcolor = { 0,0,0,0 };

	for (float j = -2.0f; j < 3.0f; j = j + 1.0f)
	{
		for (float i = -2.0f; i < 3.0f; i = i + 1.0f)
		{
			totalcolor += tex.Sample(smp, input.uv + float2(i * U, j * V));
		}
	}
return float4(totalcolor.rgb / 25.0f, 1);
}

