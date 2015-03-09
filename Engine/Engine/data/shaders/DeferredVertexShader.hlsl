cbuffer MatrixBuffer : register(cb0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float4 BlendWeights : BLENDWEIGHT;
	uint4 BlendIndices : BLENDINDICES0;
};

VS_IN VSMain(VS_IN input)
{
	return input;
}