
struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float2 Tex2 : TEXCOORD1;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
};

VS_IN VSMain(VS_IN input)
{
	return input;
}