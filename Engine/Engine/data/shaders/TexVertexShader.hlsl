cbuffer MatrixBuffer
{
	float4x4 mWorldViewProj;
	float4x4 mWorldView;
};

struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
};
struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

VS_OUT VSMain(VS_IN input)
{
	VS_OUT output;

	//output.Pos = mul(float4(input.Pos, 1.0f), mWorldViewProj);
	output.Pos = float4(input.Pos,1.0f);
	output.Tex = input.Tex;

	return output;
}