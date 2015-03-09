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
	float4x4 viewProj;
	float4x4 view;
	float4x4 proj;
};

[instance(1)]
[maxvertexcount(3)]
void GSMain(
	point GS_IN input[1],
	uint instanceID : SV_GSInstanceID,
	inout TriangleStream< GS_OUT > OutputStream
)
{
	int i = 0;
	float4 front[4] = 
	{
		{ float4(input[0].Pos.xyz, 1) + float4(-1, 1, -1, 0)*input[0].Pos.w },
		{ float4(input[0].Pos.xyz, 1) + float4(1, 1, -1, 0)*input[0].Pos.w },
		{ float4(input[0].Pos.xyz, 1) + float4(-1, -1, -1, 0)*input[0].Pos.w },
		{ float4(input[0].Pos.xyz, 1) + float4(1, -1, -1, 0)*input[0].Pos.w }
	};
	float4 back[4] =
	{
		{ float4(input[0].Pos.xyz, 1) + float4(1, 1, 1, 0)*input[0].Pos.w },
		{ float4(input[0].Pos.xyz, 1) + float4(-1, 1, 1, 0)*input[0].Pos.w },
		{ float4(input[0].Pos.xyz, 1) + float4(-1, -1, 1, 0)*input[0].Pos.w },
		{ float4(input[0].Pos.xyz, 1) + float4(1, -1, 1, 0)*input[0].Pos.w }
	};

	for (i = 0; i < 4; i++)
	{
		front[i] = mul(front[i], viewProj);
		front[i] /= front[i].w;
		clamp(front[i], -1.0f, 1.0f);
	}
	for (i = 0; i < 4; i++)
	{
		back[i] = mul(back[i], viewProj);
		back[i] /= back[i].w;
		clamp(back[i], -1.0f, 1.0f);
	}

	float4 fmax = front[0];
		float4 fmin = front[0];

	for (i = 1; i < 4; i++)
	{
		fmax = max(fmax, front[i]);
		fmin = min(fmin, front[i]);
	}
	for (i = 1; i < 4; i++)
	{
		fmax = max(fmax, back[i]);
		fmin = min(fmin, back[i]);
	}

	GS_OUT uPos1;
	GS_OUT uPos2;
	GS_OUT uPos3;
	GS_OUT uPos4;

	//float4 vPos = mul(float4(input[0].Pos.xyz,1), view);

		//float4 pos1 = vPos + float4(-input[0].Pos.w, input[0].Pos.w, 0, 0);
		uPos1.Tex = float2(0, 0);

		//float4 pos2 = vPos + float4(input[0].Pos.w, input[0].Pos.w, 0, 0);
			uPos2.Tex = float2(1, 0);

		//float4 pos3 = vPos + float4(-input[0].Pos.w, -input[0].Pos.w, 0, 0);
			uPos3.Tex = float2(0, 1);

		//float4 pos4 = vPos + float4(input[0].Pos.w, -input[0].Pos.w, 0, 0);
			uPos4.Tex = float2(1, 1);

		/*float4 pos1 = vPos + float4(-50, 50, 0, 0);

		float4 pos2 = vPos + float4(50, 50, 0, 0);

		float4 pos3 = vPos + float4(-50, -50, 0, 0);

		float4 pos4 = vPos + float4(50, -50, 0, 0);*/

			//uPos1.Pos = float4(fmin.x, fmax.y, 0, 0);

			//uPos2.Pos = float4(fmax.x, fmax.y, 0, 0);

			//uPos3.Pos = float4(fmin.x, fmin.y, 0, 0);

			//uPos4.Pos = float4(fmax.x, fmin.y, 0, 0);

			uPos1.Pos = float4(-1, 1, 0, 0)*1;

			uPos2.Pos = float4(1, 1, 0, 0)*1;

			uPos3.Pos = float4(-1, -1, 0, 0)*1;

			uPos4.Pos = float4(1, -1, 0, 0)*1;

	/*	uPos1.Pos = mul(pos1, proj);
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
*/

		//uPos1.Pos = pos1;// mul(pos1, proj);
		uPos1.Color = input[0].Color;
		uPos1.vPos = input[0].Pos;

		//uPos2.Pos = pos2;//mul(pos2, proj);
		uPos2.Color = input[0].Color;
		uPos2.vPos = input[0].Pos;

		//uPos3.Pos = pos3;//mul(pos3, proj);
		uPos3.Color = input[0].Color;
		uPos3.vPos = input[0].Pos;

		//uPos4.Pos = pos4;//mul(pos4, proj);
		uPos4.Color = input[0].Color;
		uPos4.vPos = input[0].Pos;


	//OutputStream.Append(uPos1);
	//OutputStream.Append(uPos2);
	//OutputStream.Append(uPos3);
	//OutputStream.RestartStrip();
	OutputStream.Append(uPos3);
	OutputStream.Append(uPos2);
	OutputStream.Append(uPos4);





}
