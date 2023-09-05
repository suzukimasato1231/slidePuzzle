#include "ObjShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    //���C�g
    //float3 lightdir = float3(1, -1, 1);
    //lightdir = normalize(lightdir);
    //���C�g�̐F
    //float3 lightcolor = float3(1, 1, 1);
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);

    VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(mul(viewproj, world), pos);
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = uv;
    return output;
}

PSOutput VSShadowMain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);

    PSOutput output;
    output.svpos = mul(mul(viewproj, world), pos);
    //output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = uv;
    //output.uv = 0.9f;

    output.posInLVP = mul(lightViewProj, wpos);

    return output;
}