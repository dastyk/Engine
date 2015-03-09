struct GS_IN
{
	float4 Pos : POSITION;
	float3 Color : COLOR;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
	float4 vPos : POSITION;
};
cbuffer MatrixBuffer : register(b0)
{
	float4x4 proj;
	float4x4 view;
};

[instance(1)]
[maxvertexcount(6)]
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

	float4 vPos = mul(float4(input[0].Pos.xyz, 1),view);

		//float4 pos1 = vPos + float4(-input[0].Pos.w, input[0].Pos.w, 0, 0);

		//float4 pos2 = vPos + float4(input[0].Pos.w, input[0].Pos.w, 0, 0);

		//float4 pos3 = vPos + float4(-input[0].Pos.w, -input[0].Pos.w, 0, 0);

		//float4 pos4 = vPos + float4(input[0].Pos.w, -input[0].Pos.w, 0, 0);

		float4 pos1 =  float4(-1, 1, 0, 0);

		float4 pos2 = float4(1, 1, 0, 0);

		float4 pos3 = float4(-1, -1, 0, 0);

		float4 pos4 =  float4(1, -1, 0, 0);

		/*uPos1.Pos = mul(pos1, proj);
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
		uPos4.vPos = input[0].Pos;*/


		uPos1.Pos = pos1;// mul(pos1, proj);
		uPos1.Color = input[0].Color;
		uPos1.vPos = input[0].Pos;

		uPos2.Pos = pos2;//mul(pos2, proj);
		uPos2.Color = input[0].Color;
		uPos2.vPos = input[0].Pos;

		uPos3.Pos = pos3;//mul(pos3, proj);
		uPos3.Color = input[0].Color;
		uPos3.vPos = input[0].Pos;

		uPos4.Pos = pos4;//mul(pos4, proj);
		uPos4.Color = input[0].Color;
		uPos4.vPos = input[0].Pos;


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
