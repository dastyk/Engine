cbuffer MatrixBuffer : register(cb0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
	float4x4 mWorldView;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	uint Id : BLENDINDICES;
};
struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float4 PosH : POSITION;
	uint Id : BLENDINDICES;
};

VS_OUT VSMain(VS_IN input)
{
	VS_OUT output;

	output.Tex = input.Tex;
	output.Pos = mul(float4(input.Pos, 1.0f), mWorldViewProj);
	output.PosH = mul(float4(input.Pos, 1.0f),mWorld);
	output.Normal = normalize(mul(float4(input.Normal,1),mWorld));
	output.Id = input.Id;

	return output;
}