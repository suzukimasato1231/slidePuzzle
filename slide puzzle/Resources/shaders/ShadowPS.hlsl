#include "ObjShaderHeader.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> shadow : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 PSmain(VSOutput input) : SV_TARGET
{
    //�e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv);

    //�V�F�[�f�B���O�ɂ��F
    float4 shadecolor;
    //����x
    const float shininess = 4.0f;
    //���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
    //���C�g�Ɍ������x�N�g���Ɩ@���̓���
    float3 dotlightnormal = dot(lightv, input.normal);
    //���ˌ��x�N�g��
    float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
    //�����ˌ�
    float3 ambient = (1, 1, 1); /*m_ambient*/
    //�g�U���ˌ�
    float3 diffuse = dotlightnormal * m_diffuse * 3.14f;
    //���ʔ��ˌ�
    float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

    //���Z
    shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
    shadecolor.a = m_alpha;

    return shadecolor * texcolor;
}

float4 PSShadowMain(PSOutput input) : SV_TARGET
{
    float4 texcolor = shadow.Sample(smp, float2(0.5, 0.5));
    return float4(texcolor.rgb, 1.0);

    float2 shadowMapUV = input.posInLVP.xy / input.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    float3 shadowMap = 1.0f;

    //�͈͓�
    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        shadowMap = shadow.Sample(smp, shadowMapUV);

    }

    texcolor.xyz *= shadowMap;

    return texcolor;
}

float4 PSBlack() : SV_TARGET
{
    return float4(0.5f, 0.5f, 0.5f, 1.0f);
}