cbuffer MatrixBuffer
{
	float4x4 proj;
	float4x4 view;
};

struct GS_IN
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
};


void CreateQuad(GS_IN inVerts[1], out GS_IN outVerts[6])
{

	float4 posH = mul(inVerts[0].Pos, view);
	posH = posH / posH.w;
	float3 N = normalize(-posH.xyz);
	float3 R = normalize(cross(float3(0, 1, 0), N));
	float3 U = cross(N, R);

	outVerts[0].Pos = mul(float4(inVerts[0].Pos.xyz - R + U, 1), proj);
	outVerts[0].Color = inVerts[0].Color;

	outVerts[1].Pos = mul(float4(inVerts[0].Pos.xyz + R + U, 1), proj);
	outVerts[1].Color = inVerts[0].Color;

	outVerts[3].Pos = outVerts[2].Pos = mul(float4(inVerts[0].Pos.xyz - R - U, 1), proj);
	outVerts[3].Color = outVerts[2].Color = inVerts[0].Color;

	outVerts[4].Pos = mul(float4(inVerts[0].Pos.xyz + R + U, 1), proj);
	outVerts[4].Color = inVerts[0].Color;

	outVerts[5].Pos = mul(float4(inVerts[0].Pos.xyz + R - U, 1), proj);
	outVerts[5].Color = inVerts[0].Color;


}

void outputData(GS_IN v[6], inout TriangleStream<GS_OUT> triStream)
{
	GS_OUT output = (GS_OUT)0;
	uint i = 0;
	for ( i = 0; i < 3; i++)
	{
		output.Pos = v[i].Pos;
		output.Color = v[i].Color;

		triStream.Append(output);
	}
	triStream.RestartStrip();

	for (i = 0; i < 3; i++)
	{
		output.Pos = v[3+i].Pos;
		output.Color = v[3+i].Color;

		triStream.Append(output);
	}
	triStream.RestartStrip();
}

[instance(1)]
[maxvertexcount(6)]
void GSMain(
	point GS_IN input[1],
	uint instanceID : SV_GSInstanceID,
	inout TriangleStream< GS_OUT > OutputStream
)
{

	GS_OUT output;

	//CreateQuad(input, output);

	//outputData(output, OutputStream);

	/*float4 pos1 = float4(-1, 1, 0, 1);
	float4 pos2 = float4(1, 1, 0, 1);
	float4 pos3 = float4(-1, -1, 0, 1);

	float4 pos4 = float4(-1, -1, 0, 1);
	float4 pos5 = float4(1, 1, 0, 1);
	float4 pos6 = float4(1, -1, 0, 1);*/

	float4 posH = mul(input[0].Pos, view);
		posH = posH / posH.w;

	float4 pos1 = posH + float4(-0.05, 0.05, 0, 0);
		float4 pos2 = posH + float4(0.05, 0.05, 0, 0);
		float4 pos3 = posH + float4(-0.05, -0.05, 0, 0);

		float4 pos4 = posH + float4(0.05, -0.05, 0, 0);

	output.Pos = mul(pos1, proj);
	output.Color = input[0].Color;

	OutputStream.Append(output);

	output.Pos = mul(pos2,  proj);
	output.Color = input[0].Color;

	OutputStream.Append(output);

	output.Pos = mul(pos3, proj);
	output.Color = input[0].Color;

	OutputStream.Append(output);

	OutputStream.RestartStrip();

	output.Pos = mul(pos3,  proj);
	output.Color = input[0].Color;

	OutputStream.Append(output);

	output.Pos = mul(pos2, proj);
	output.Color = input[0].Color;

	OutputStream.Append(output);

	output.Pos = mul(pos4,proj);
	output.Color = input[0].Color;

	OutputStream.Append(output);

	




}
