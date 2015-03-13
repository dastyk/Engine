cbuffer MatrixBuffer : register(c0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
};



struct VS_IN
{
	float3 Pos : POSITION;
};
struct VS_OUT
{
	float4 Pos : SV_POSITION;
};

float4 VSMain(VS_IN input) :SV_POSITION
{
	return mul(float4(input.Pos, 1.0f), mWorldViewProj);
}