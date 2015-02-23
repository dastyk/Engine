cbuffer MatrixBuffer : register(cb0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
};


struct GS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 PosH : POSITION;
	float2 Tex2 : TEXCOORD1;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 PosH : POSITION;
	float2 Tex2 : TEXCOORD1;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
};


void outputData(GS_IN v[3], inout TriangleStream<GS_OUT> triStream)
{
	GS_OUT output = (GS_OUT)0;

	for (uint i = 0; i < 3; i++)
	{
		output.Pos = mul(v[i].Pos, mWorldViewProj);
		output.Tex = v[i].Tex;
		output.Tex2 = v[i].Tex2;
		output.Normal = normalize(mul(float4(v[i].Normal, 0), mWorld).xyz);	
		float4 temp = mul(float4(v[i].PosH, 0), mWorld);

		output.PosH = temp.xyz;

		output.Tangent = normalize(mul(float4(v[i].Tangent, 0), mWorld).xyz);
		output.Binormal = normalize(mul(float4(v[i].Binormal, 0), mWorld).xyz);

		triStream.Append(output);
	}
	triStream.RestartStrip();
}

[instance(1)]
[maxvertexcount(3)]
void GSMain(triangle GS_IN input[3],uint instanceID : SV_GSInstanceID, inout TriangleStream<GS_OUT> OutputStream)
{

	for (uint i = 0; i < 3; i++)
	{

		input[i].Pos.y += 3 * (instanceID);
		input[i].PosH.y += 3 * (instanceID);

	}

	outputData(input, OutputStream);



}

