
#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0);//0�ԃX���b�g�Ɏw�肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);//�O�ԃX���b�g�Ɏw�肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{

	return tex.Sample(smp,input.uv) * color;

}

