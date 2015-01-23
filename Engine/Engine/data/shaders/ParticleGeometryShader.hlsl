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
	float2 Tex : TEXCOORD0;
};


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
	float2 tex1 = float2(0, 0);

	float4 pos2 = posH + float4(0.05, 0.05, 0, 0);
	float2 tex2 = float2(1, 0);

	float4 pos3 = posH + float4(-0.05, -0.05, 0, 0);
	float2 tex3 = float2(0, 1);

	float4 pos4 = posH + float4(0.05, -0.05, 0, 0);
	float2 tex4 = float2(1, 1);

	output.Pos = mul(pos1, proj);
	output.Color = input[0].Color;
	output.Tex = tex1;

	OutputStream.Append(output);

	output.Pos = mul(pos2,  proj);
	output.Color = input[0].Color;
	output.Tex = tex2;

	OutputStream.Append(output);

	output.Pos = mul(pos3, proj);
	output.Color = input[0].Color;
	output.Tex = tex3;

	OutputStream.Append(output);

	OutputStream.RestartStrip();

	OutputStream.Append(output);

	output.Pos = mul(pos2, proj);
	output.Color = input[0].Color;
	output.Tex = tex2;

	OutputStream.Append(output);

	output.Pos = mul(pos4,proj);
	output.Color = input[0].Color;
	output.Tex = tex4;

	OutputStream.Append(output);

	




}
