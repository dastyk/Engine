cbuffer MatrixBuffer : register(cb0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorld;
	float4x4 mWorldView;
};


struct GS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float4 BlendWeights : BLENDWEIGHT;
	uint4 BlendIndices : BLENDINDICES0;
	uint Id : BLENDINDICES1;
	float3 PosH : POSITION;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float4 BlendWeights : BLENDWEIGHT;
	uint4 BlendIndices : BLENDINDICES0;
	uint Id : BLENDINDICES1;
	float3 PosH : POSITION;
};


void outputData(GS_IN v[3], inout TriangleStream<GS_OUT> triStream)
{
	GS_OUT output = (GS_OUT)0;

	for (uint i = 0; i < 3; i++)
	{
		output.Pos = mul(v[i].Pos, mWorldViewProj);
		output.Tex = v[i].Tex;
		
		float4 temp = mul(float4(v[i].PosH, 1), mWorld);

			output.PosH = temp.xyz / temp.w;
		temp = mul(float4(v[i].Normal, 1), mWorld);
		output.Normal = normalize(temp.xyz/temp.w);
		output.BlendWeights = v[i].BlendWeights;
		output.BlendIndices = v[i].BlendIndices;
		output.Id = v[i].Id;

		triStream.Append(output);
	}
	triStream.RestartStrip();
}

[instance(1)]
[maxvertexcount(3)]
void GSMain(triangle GS_IN input[3],uint instanceID : SV_GSInstanceID, inout TriangleStream<GS_OUT> OutputStream)
{
	//GS_IN v[6];


	//CopyTriangle(input, v);

	for (uint i = 0; i < 3; i++)
	{

		input[i].Pos.y += 3 * (instanceID);
		input[i].PosH.y += 3 * (instanceID);

	}

	outputData(input, OutputStream);



}

