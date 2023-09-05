
#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0);//0番スロットに指定されたテクスチャ
SamplerState smp : register(s0);//０番スロットに指定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{

	return tex.Sample(smp,input.uv) * color;

}

