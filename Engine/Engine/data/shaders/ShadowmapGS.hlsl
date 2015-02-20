cbuffer MatrixBuffer : register(c0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
	float4 CamPos;
};




struct GS_IN
{
	float4 Pos : SV_POSITION;
};
struct GS_OUT
{
	float4 Pos : SV_POSITION;
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

			ts.Append(output);
		}

		ts.RestartStrip();
	}
}
