
#include "DepthOfField.hlsli"

Texture2D<float4> tex : register(t0);//0番スロットに指定されたテクスチャ
SamplerState smp : register(s0);//０番スロットに指定されたサンプラー
Texture2D<float> tex2 : register(t1);//1番スロットに指定されたテクスチャ

float4 main(VSOutput input) : SV_TARGET
{
//深度値
float depth = tex2.Sample(smp, input.uv);
//U座標の変化量を変数に入れる
float U = 1.0f / 1200.0f;
//V座標の変化量を変数に入れる
float V = 1.0f / 720.0f;
//初期化
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

