
#include "ShadowMap.hlsli"

Texture2D<float> tex : register(t0);//0番スロットに指定されたテクスチャ
SamplerState smp : register(s0);//０番スロットに指定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{

	float4 totalcolor = tex.Sample(smp, input.uv);

	return float4(totalcolor.rgb, 1);
}

