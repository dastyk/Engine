cbuffer ConstantB : register(c0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
	float4x4 LightViewProj;
};


struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float2 Tex2 : TEXCOORD1;
};
struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 PosH : POSITION0;
	float2 Tex2 : TEXCOORD1;
	float4 LPos : POSITION1;
};

VS_OUT VSMain(VS_IN input)
{
	VS_OUT output;

	output.Pos = mul(float4(input.Pos,1), mWorldViewProj);
	output.Tex = input.Tex;
	output.Tex2 = input.Tex2;
	output.Normal = normalize(mul(float4(input.Normal, 0), mWorld).xyz);
	float4 temp = mul(float4(input.Pos, 1), mWorld);
	output.PosH = temp.xyz;
	output.LPos = mul(float4(input.Pos, 1), LightViewProj);

	return output;
}