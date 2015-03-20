cbuffer ConstantB : register(c0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
	float3 CamPos;
};

struct GS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float4 BlendWeights : BLENDWEIGHT;
	uint4 BlendIndices : BLENDINDICES0;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float4 BlendWeights : BLENDWEIGHT;
	uint4 BlendIndices : BLENDINDICES0;
	float3 PosH : POSITION;

};

[maxvertexcount(3)]
void GSMain(
	triangle GS_IN input[3],
	inout TriangleStream< GS_OUT > ts
)
{
	float3 P[3];
	for (uint i = 0; i < 3; i++)
	{
		
		P[i] = mul(float4(input[i].Pos, 1), mWorld).xyz;
	}

	float3 v1 = P[1] - P[0];
		float3 v2 = P[2] - P[0];

		float3 N = normalize(cross(v1, v2));

		float3 C = 3 * CamPos - P[0] - P[1] - P[2];
		//float3 C = 3 * float3(128, 25, 128) - P[0] - P[1] - P[2];

		float v = dot(N, C);
	if (v >= 0)
	{

		for (uint i = 0; i < 3; i++)
		{
			GS_OUT output;
			output.Pos = mul(float4(input[i].Pos, 1), mWorldViewProj);
			output.Tex = input[i].Tex;
			output.Normal = normalize(mul(float4(input[i].Normal, 0), mWorld).xyz);
			output.PosH = mul(float4(input[i].Pos, 1), mWorld).xyz;
			output.BlendWeights = input[i].BlendWeights;
			output.BlendIndices = input[i].BlendIndices;

			ts.Append(output);
		}

		ts.RestartStrip();
	}
}
