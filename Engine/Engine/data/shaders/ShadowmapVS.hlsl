cbuffer MatrixBuffer : register(c0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
};



struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float4 BlendWeights : BLENDWEIGHT;
	uint4 BlendIndices : BLENDINDICES0;
	uint Id : BLENDINDICES1;
};
struct VS_OUT
{
	float4 Pos : SV_POSITION;
};

float4 VSMain(VS_IN input) :SV_POSITION
{
	return mul(float4(input.Pos, 1.0f), mWorldViewProj);
}