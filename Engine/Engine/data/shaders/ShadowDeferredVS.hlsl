cbuffer ConstantB
{
	float4x4 LightViewProj;
	float3 lightPos;
};


struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
};
struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 LPos : POSITION;
};

VS_OUT VSMain(VS_IN input)
{
	VS_OUT output;

	output.Tex = input.Tex;
	output.Pos = float4(input.Pos, 1.0f); 
	output.LPos = mul(float4(lightPos, 1), LightViewProj).xyz;

	return output;
}