struct GS_IN
{
	float4 Pos : POSITION;
	float3 Color : COLOR;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Color : COLOR;
	float4 vPos : POSITION;
};
cbuffer MatrixBuffer
{
	float4x4 view;
	float4x4 proj;
};

[instance(1)]
[maxvertexcount(36)]
void GSMain(
	point GS_IN input[1],
	uint instanceID : SV_GSInstanceID,
	inout TriangleStream< GS_OUT > OutputStream
)
{
	GS_OUT uPos1;
	GS_OUT uPos2;
	GS_OUT uPos3;
	GS_OUT uPos4;

	float4 vPos = mul(float4(input[0].Pos.xyz,1), view);

		float4 pos1 = vPos + float4(-input[0].Pos.w, input[0].Pos.w, 1, 0);
		uPos1.Tex = float2(0, 0);

	float4 pos2 = vPos + float4(input[0].Pos.w, input[0].Pos.w, 1, 0);
			uPos2.Tex = float2(1, 0);

	float4 pos3 = vPos + float4(-input[0].Pos.w, -input[0].Pos.w, 1, 0);
			uPos3.Tex = float2(0, 1);

	float4 pos4 = vPos + float4(input[0].Pos.w, -input[0].Pos.w, 1, 0);
			uPos4.Tex = float2(1, 1);


	uPos1.Pos = mul(pos1, proj);
	uPos1.Color = input[0].Color;
	uPos1.vPos = input[0].Pos;

		uPos2.Pos = mul(pos2, proj);
		uPos2.Color = input[0].Color;
		uPos2.vPos = input[0].Pos;

		uPos3.Pos = mul(pos3, proj);
		uPos3.Color = input[0].Color;
		uPos3.vPos = input[0].Pos;

		uPos4.Pos = mul(pos4, proj);
		uPos4.Color = input[0].Color;
		uPos4.vPos = input[0].Pos;


	OutputStream.Append(uPos1);
	OutputStream.Append(uPos2);
	OutputStream.Append(uPos3);
	//OutputStream.RestartStrip();
	OutputStream.Append(uPos3);
	OutputStream.Append(uPos2);
	OutputStream.Append(uPos4);





}
