#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput
{
	float4 target0 :SV_TARGET0;
	float4 target1 :SV_TARGET1;
};

PSOutput main(VSOutput input)
{
	PSOutput output;
	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);

	// ����x
	const float shininess = 4.0f;
	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.svpos.xyz);

	// �����ˌ�
	float3 ambient = 0.5;

	// �V�F�[�f�B���O�ɂ��F
	float4 shadecolor = float4(ambientColor * 0.5, 1.0);

	//���s����
	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			// ���ˌ��x�N�g��
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			// �g�U���ˌ�
			float3 diffuse = dotlightnormal * 0.3;
			// ���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * 0.3;

			// �S�ĉ��Z����
			shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
		}
	}

	// �A�e�ƃe�N�X�`���̐F������
	output.target0 = shadecolor * texcolor;
	output.target1 = float4(1 - (shadecolor * texcolor).rgb, 1);

	return output;
}