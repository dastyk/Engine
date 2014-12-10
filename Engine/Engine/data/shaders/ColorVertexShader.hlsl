cbuffer MatrixBuffer
{
	float4x4 mWorldViewProj;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
};
struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

VS_OUT VSMain( VS_IN input)
{
	VS_OUT output;

	output.Pos = mul(float4(input.Pos,1.0f),mWorldViewProj);
	//output.Pos = float4(input.Pos,1.0f);
	output.Color = input.Color;

	return output;
}