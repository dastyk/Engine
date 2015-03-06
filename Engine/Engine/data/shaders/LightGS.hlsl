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
	float2 Tex : TEXCOORD;
};


[instance(1)]
[maxvertexcount(6)]
void GSMain(
	point float4 input[1] : SV_POSITION,
	uint instanceID : SV_GSInstanceID,
	inout TriangleStream< GS_OUT > OutputStream
)
{
	GS_OUT uPos1;
	GS_OUT uPos2;
	GS_OUT uPos3;
	GS_OUT uPos4;

	float4 vPos = mul(float4(128, 24, 128, 0), view);

		/*float4 pos1 = vPos + float4(-Color_LightRange.w, Color_LightRange.w, 0, 0);

		float4 pos2 = vPos + float4(Color_LightRange.w, Color_LightRange.w, 0, 0);

		float4 pos3 = vPos + float4(-Color_LightRange.w, -Color_LightRange.w, 0, 0);

		float4 pos4 = vPos + float4(Color_LightRange.w, -Color_LightRange.w, 0, 0);*/

		float4 pos1 = vPos + float4(-20, 20, 0, 0);
		float2 tex1 = float2(0, 0);
		float4 pos2 = vPos + float4(20, 20, 0, 0);
		float2 tex2 = float2(1, 0);
		float4 pos3 = vPos + float4(-20, -20, 0, 0);
		float2 tex3 = float2(0,1);
		float4 pos4 = vPos + float4(20, -20, 0, 0);
		float2 tex4 = float2(1, 1);
		/*float4 pos1 = float4(-1, 1, 0, 0);

		float4 pos2 = float4(1, 1, 0, 0);

		float4 pos3 = float4(-1, -1, 0, 0);

		float4 pos4 = float4(1, -1, 0, 0);*/

		uPos1.Pos = mul(pos1, proj);
		uPos2.Pos = mul(pos2, proj);
		uPos3.Pos = mul(pos3, proj);
		uPos4.Pos = mul(pos4, proj);

		uPos1.Pos = pos1;
	uPos2.Pos = pos2;
	uPos3.Pos = pos3;
	uPos4.Pos = pos4;
	uPos1.Tex = tex1;
	uPos2.Tex = tex2;
	uPos3.Tex = tex3;
	uPos4.Tex = tex4;

	OutputStream.Append(uPos1);
	OutputStream.Append(uPos2);
	OutputStream.Append(uPos3);
	OutputStream.RestartStrip();
	OutputStream.Append(uPos3);
	OutputStream.Append(uPos2);
	OutputStream.Append(uPos4);

/*
	output.Pos = mul(float4(pos1,1), proj);

	OutputStream.Append(output);

	output.Pos = mul(float4(pos2, 1), proj);


	OutputStream.Append(output);

	output.Pos = mul(float4(pos3, 1), proj);


	OutputStream.Append(output);

	OutputStream.RestartStrip();

	OutputStream.Append(output);

	output.Pos = mul(float4(pos2, 1), proj);

	OutputStream.Append(output);

	output.Pos = mul(float4(pos4, 1), proj);

	OutputStream.Append(output);
*/
	




}
