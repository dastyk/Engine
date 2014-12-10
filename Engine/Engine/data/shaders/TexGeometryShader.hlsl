cbuffer MatrixBuffer
{
	float4x4 mWorldViewProj;
	float4x4 mWorldView;
};

struct GS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
};


void CopyTriangle(GS_IN inVerts[3], out GS_IN outVerts[9])
{

	for (uint i = 0; i < 3; i++)
	{
		outVerts[i] = inVerts[i];
		outVerts[i + 3] = inVerts[i];
		outVerts[i + 6] = inVerts[i];
	}

}

void outputData(GS_IN v[9], inout TriangleStream<GS_IN> triStream)
{
	uint i;
	GS_IN output = (GS_IN)0;
	for ( i = 0; i < 3; i++)
	{
		output.Pos = mul(v[i].Pos, mWorldViewProj);
		output.Tex = v[i].Tex;
		triStream.Append(output);
	}
	triStream.RestartStrip();

	/*for ( i = 3; i < 6; i++)
	{
		
		output.Pos = mul(v[i].Pos, mWorldViewProj);
		output.Pos.y += 3;
		output.Tex = v[i].Tex;
		triStream.Append(output);
	}
	triStream.RestartStrip();

	for (i = 6; i < 9; i++)
	{
		output.Pos = mul(v[i].Pos, mWorldViewProj);
		output.Pos.y -= 3;
		output.Tex = v[i].Tex;
		triStream.Append(output);
	}
	triStream.RestartStrip();*/
}

[instance(1)]
[maxvertexcount(3)]
void GSMain(triangle GS_IN input[3], inout TriangleStream<GS_IN> OutputStream)
{
	GS_IN v[9];
	GS_OUT output[3];

	CopyTriangle(input, v);


	outputData(v, OutputStream);

	
	
}

