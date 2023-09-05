
#include "DepthOfField.hlsli"

Texture2D<float4> tex : register(t0);//0�ԃX���b�g�Ɏw�肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);//�O�ԃX���b�g�Ɏw�肳�ꂽ�T���v���[
Texture2D<float> tex2 : register(t1);//1�ԃX���b�g�Ɏw�肳�ꂽ�e�N�X�`��

float4 main(VSOutput input) : SV_TARGET
{
//�[�x�l
float depth = tex2.Sample(smp, input.uv);
//U���W�̕ω��ʂ�ϐ��ɓ����
float U = 1.0f / 1200.0f;
//V���W�̕ω��ʂ�ϐ��ɓ����
float V = 1.0f / 720.0f;
//������
float4 totalcolor = { 0,0,0,0 };
//if (depth >= 0.999500f)
//{
//	for (float j = -1.0f; j < 2.0f; j = j + 1.0f)
//	{
//		for (float i = -1.0f; i < 2.0f; i = i + 1.0f)
//		{
//			totalcolor += tex.Sample(smp, input.uv + float2(i * U, j * V));
//		}
//	}
//	totalcolor.rgb /= 9.0;
//}
//else
{
	totalcolor += tex.Sample(smp, input.uv);
}
	return float4(totalcolor.rgb, 1);
}

