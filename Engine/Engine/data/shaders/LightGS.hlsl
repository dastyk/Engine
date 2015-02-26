cbuffer MatrixBuffer : register(b0)
{
	float4x4 proj;
	float4x4 view;
};

cbuffer PointLight : register(b1)
{
	float3 LPos;

	float4 Color_LightRange;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
};


[instance(1)]
[maxvertexcount(6)]
void GSMain(
	point float4 input[1] : SV_POSITION,
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

	float4 posH = mul(float4(128,50,128,1), view);
	//posH = posH / posH.w;

	float4 pos1 = posH + float4(-Color_LightRange.w, Color_LightRange.w, 0, 0);

	float4 pos2 = posH + float4(Color_LightRange.w, Color_LightRange.w, 0, 0);

	float4 pos3 = posH + float4(-Color_LightRange.w, -Color_LightRange.w, 0, 0);

	float4 pos4 = posH + float4(Color_LightRange.w, -Color_LightRange.w, 0, 0);

	output.Pos = mul(pos1, proj);

	OutputStream.Append(output);

	output.Pos = mul(pos2,  proj);


	OutputStream.Append(output);

	output.Pos = mul(pos3, proj);


	OutputStream.Append(output);

	OutputStream.RestartStrip();

	OutputStream.Append(output);

	output.Pos = mul(pos2, proj);

	OutputStream.Append(output);

	output.Pos = mul(pos4,proj);

	OutputStream.Append(output);

	




}
