#include "FBXShadow.hlsli"

struct PSOutput
{
	float4 target0 :SV_TARGET0;
	float4 target1 :SV_TARGET1;
};

PSOutput main(VSOutput input)
{
	PSOutput output;
	
	output.target0 = float4(1,1,1,1);
	output.target1 = float4(1,1,1,1); 
	return output;
}