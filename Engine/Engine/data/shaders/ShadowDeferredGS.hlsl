cbuffer ConstantB : register(c0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
	float4x4 LightViewProj;
	float3 CamPos;
};



struct GS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float2 Tex2 : TEXCOORD1;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
};
struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 PosH : POSITION0;
	float2 Tex2 : TEXCOORD1;
	float4 LPos : POSITION1;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
};

[maxvertexcount(3)]
void GSMain(
	triangle GS_IN input[3],
	inout TriangleStream< GS_OUT > ts
)
{

	float3 v1 = input[1].Pos.xyz - input[0].Pos.xyz;
		float3 v2 = input[2].Pos.xyz - input[0].Pos.xyz;

		float3 N = normalize(cross(v1, v2));

		float3 C = 3 * CamPos - input[0].Pos.xyz - input[1].Pos.xyz - input[2].Pos.xyz;

		float v = dot(N, C);
	if (v >= 0)
	{

		for (uint i = 0; i < 3; i++)
		{
			GS_OUT output;
			output.Pos = mul(float4(input[i].Pos,1), mWorldViewProj);
			output.Tex = input[i].Tex;
			output.Tex2 = input[i].Tex2;
			output.Normal = normalize(mul(float4(input[i].Normal, 0), mWorld).xyz);
			float4 temp = mul(float4(input[i].Pos, 1), mWorld);
				output.PosH = temp.xyz;
			output.LPos = mul(float4(input[i].Pos, 1), LightViewProj);
			output.Tangent = normalize(mul(float4(input[i].Tangent, 0), mWorld).xyz);
			output.Binormal = normalize(mul(float4(input[i].Binormal, 0), mWorld).xyz);

			ts.Append(output);
		}

		ts.RestartStrip();
	}


	//for (uint i = 0; i < 3; i++)
	//{
	//	GS_OUT output;
	//	output.Pos = mul(input[i].Pos, mWorldViewProj);
	//	output.Tex = input[i].Tex;
	//	output.Tex2 = input[i].Tex2;
	//	output.Normal = normalize(mul(float4(input[i].Normal, 0), mWorld).xyz);
	//	float4 temp = mul(input[i].Pos, mWorld);
	//		output.PosH = temp.xyz;
	//	output.LPos = float4(0, 0, 0, 0);// mul(input[i].Pos, LightViewProj);
	//	output.Tangent = float3(0, 0, 0);// normalize(mul(float4(input[i].Tangent, 0), mWorld).xyz);
	//	output.Binormal = float3(0, 0, 0);//normalize(mul(float4(input[i].Binormal, 0), mWorld).xyz);

	//	ts.Append(output);
	//}

	ts.RestartStrip();
}
