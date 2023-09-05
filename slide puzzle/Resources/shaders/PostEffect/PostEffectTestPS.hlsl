
#include "PostEffectTest.hlsli"

Texture2D<float4> tex : register(t0);//0番スロットに指定されたテクスチャ
SamplerState smp : register(s0);//０番スロットに指定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//U座標の変化量を変数に入れる
	float U = 1.0f / 1200.0f;
	//V座標の変化量を変数に入れる
	float V = 1.0f / 720.0f;
	//初期化
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

