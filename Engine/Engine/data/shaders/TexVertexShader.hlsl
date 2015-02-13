cbuffer MatrixBuffer : register(cb0)
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
	float2 Tex : TEXCOORD;

};

VS_OUT VSMain(VS_IN input)
{
	VS_OUT output;

	//output.Pos = mul(float4(input.Pos, 1.0f), mWorldViewProj);
	output.Pos = mul(float4(input.Pos, 1.0f),mWorld);
	output.Tex = input.Tex;
	//output.Normal = input.Normal;
	//output.BlendWeights = input.BlendWeights;
	//output.BlendIndices = input.BlendIndices;
	//output.Id = input.Id;

	return output;
}