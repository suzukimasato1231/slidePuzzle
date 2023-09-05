#include "ShadowMap.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(lightproj, wpos);//���C�g���猩���s��
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	return output;
}