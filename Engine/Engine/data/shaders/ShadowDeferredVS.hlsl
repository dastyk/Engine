
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
	float2 Tex2 : TEXCOORD1;
};

VS_OUT VSMain(VS_IN input)
{
	VS_OUT output;

	//output.Pos = mul(float4(input.Pos,1), mWorldViewProj);
	//output.Tex = input.Tex;
	//output.Tex2 = input.Tex2;
	//output.Normal = normalize(mul(float4(input.Normal, 0), mWorld).xyz);
	//float4 temp = mul(float4(input.Pos, 1), mWorld);
	//output.PosH = temp.xyz;
	//output.LPos = mul(float4(input.Pos, 1), LightViewProj);

	output.Pos = float4(input.Pos,1);
	output.Tex = input.Tex;
	output.Tex2 = input.Tex2;
	output.Normal = input.Normal;

	return output;
}