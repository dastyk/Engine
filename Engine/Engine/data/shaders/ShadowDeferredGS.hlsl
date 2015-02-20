cbuffer ConstantB : register(c0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
	float4x4 LightViewProj;
	float4 CamPos;
};



struct GS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float2 Tex2 : TEXCOORD1;
};
struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 PosH : POSITION0;
	float2 Tex2 : TEXCOORD1;
	float4 LPos : POSITION1;
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
			output.Pos = mul(input[i].Pos, mWorldViewProj);
			output.Tex = input[i].Tex;
			output.Tex2 = input[i].Tex2;
			output.Normal = normalize(mul(float4(input[i].Normal, 0), mWorld).xyz);
			float4 temp = mul(input[i].Pos, mWorld);
				output.PosH = temp.xyz;
			output.LPos = mul(input[i].Pos, LightViewProj);
			ts.Append(output);
		}

		ts.RestartStrip();
	}
}
