#define MAX_ACTIVE_LIGHTS 10


struct PointLight
{
	float3 Pos;

	float3 ambientColor;
	

	float3 diffColor;
	

	float3 specColor;
	

	float4 LightRange_Unused_Unused_Unused;
};


cbuffer LightBuffer : register(b0)
{
	float4 LightCount_FogRange_ShinyPower_Unused;

	float3 ambientReflection;

	float3 diffReflection;

	float3 specReflection;

	float3 fogColor;

	float3 CamPos;

	PointLight lights[MAX_ACTIVE_LIGHTS];
};



Texture2D shaderTexture[2];
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};


float4 PSMain(PS_IN input) : SV_TARGET
{
	float4 Normal_Depth = shaderTexture[0].Sample(SampleType, input.Tex);
	float4 Color = shaderTexture[1].Sample(SampleType, input.Tex);

	return Color;
}
