#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp0 : register(s0);      // 0番スロットに設定されたサンプラー
Texture2D<float> shadow : register(t1);  // 1番スロットに設定されたテクスチャ


float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp0, input.uv + uvMove);

	// 光沢度
	const float shininess = 4.0f;
	// 頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// 環境反射光
	float3 ambient = m_ambient;

	// シェーディングによる色
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	//平行光源
   for (int i = 0; i < DIRLIGHT_NUM; i++) {
	   if (dirLights[i].active) {
		   // ライトに向かうベクトルと法線の内積
		   float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
		   // 反射光ベクトル
		  float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
		  // 拡散反射光
		  float3 diffuse = dotlightnormal * m_diffuse;
		  // 鏡面反射光
		  float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

		  // 全て加算する
		  shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
	  }
  }

   ////ライト視点から見た位置を求める
   float3 posFromLightVP = input.posInLVP.xyz / input.posInLVP.w;
   float2 shadowmap = (posFromLightVP.xy + float2(1,-1)) * float2(0.5,-0.5);
   //シャドウマッピングの範囲内か
   if (shadowmap.x <= 1.0f && shadowmap.x >= 0.0f &&
	   shadowmap.y <= 1.0f && shadowmap.y >= 0.0f)
   {  
		   float shadowDepth = (shadow.Sample(smp0, shadowmap)).x;
		   //深度を比較
		   if (shadowDepth < posFromLightVP.z - 0.0025f)
		   {
			   shadecolor.rgb = shadecolor.rgb * 0.6f;//暗くする
		   }
   }
   // シェーディングによる色で描画
	return shadecolor * texcolor;
}


