
cbuffer PointLight : register(b0)
{
	float3 LPos;

	float4 Color_LightRange;
};

cbuffer CamBuffer : register(b1)
{
	float3 CamPos;
};

Texture2D shaderTexture[4];
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};


float4 PSMain(PS_IN input) : SV_TARGET
{
	float4 Normal_Depth = shaderTexture[0].Sample(SampleType, input.Tex);
	float4 Color = shaderTexture[1].Sample(SampleType, input.Tex);
	float4 Pos = shaderTexture[2].Sample(SampleType, input.Tex);
	float4 Spec_SpecPow = shaderTexture[3].Sample(SampleType, input.Tex);

	//float4 finColor;
	//float3 V = CamPos - Pos.xyz;
	//	float D = length(V);
	//V = normalize(V);

	//float3 N = Normal_Depth.xyz;

	//	float3 LightCont = float3(0, 0, 0);

	//	float3 S = Pos - Pos.xyz;
	//	float dist = length(S);
	//S = normalize(S);
	//float end = Color_LightRange.w;
	//float range = saturate((end - dist) / end);

	//float3 R = reflect(-S, N);

	//	// set ambient contrib.

	//	float3 am = range*Color_LightRange.xyz*0.2;
	//	LightCont = saturate(LightCont + am);

	//// Calculate diffuse Light contrib.
	//float3 Ld = range*Color_LightRange.xyz*max(dot(S, N), 0);

	//	// Add diffuse contrib. to color.
	//	LightCont = saturate(LightCont + Ld);

	//// Calculate specular light contrib.
	//float3 Ls = Spec_SpecPow.xyz*range*Color_LightRange.xyz*pow(max(dot(R, V), 0), Spec_SpecPow.w);

	//// Add specular contrib. to color.
	//LightCont = saturate(LightCont + Ls);

	//finColor = (Color * float4(LightCont, 1.0f));


	// Diffuse

	float3 ambient = float3(0.1, 0.1, 0.1);

	float3 L = LPos - Pos.xyz;
		float dist = length(L);
	float range = saturate((Color_LightRange.w - dist) / Color_LightRange.w);
	L /= dist;

	float nDotL = saturate(dot(Normal_Depth.xyz, L));
	float3 diffuse = nDotL*Color_LightRange.xyz*float3(0.7, 0.7, 0.7);

		// Specular
		float3 V = CamPos - Pos.xyz;
		float3 H = normalize(L + V);
		float3 specular = pow(saturate(dot(Normal_Depth.xyz, H)), Spec_SpecPow.w)* Color_LightRange.xyz* Spec_SpecPow.xyz*nDotL;



		return float4(ambient + diffuse + specular, 1)*range;
}
