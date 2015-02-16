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
	float3 Color: COLOR;
};

VS_OUT VSMain(VS_IN input)
{
	VS_OUT output;
	output.Pos = mul(float4(input.Pos, 1.0f), mWorldViewProj);
	//output.Color = input.Color;
	return output;
}