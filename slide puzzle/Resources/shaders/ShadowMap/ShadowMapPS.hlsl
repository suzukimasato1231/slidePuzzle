
#include "ShadowMap.hlsli"

Texture2D<float> tex : register(t0);//0�ԃX���b�g�Ɏw�肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);//�O�ԃX���b�g�Ɏw�肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{

	float4 totalcolor = tex.Sample(smp, input.uv);

	return float4(totalcolor.rgb, 1);
}

