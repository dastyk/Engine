#define MAX_ACTIVE_LIGHTS 10
#define MAX_MATERIAL_COUNT 10


struct MatrialDesc
{
	float3 Ambient;
	float3 Diffuse;
	float3 Specular;
	float3 Reflectivity;

	float4 SpecPower_AlphaClip_Unused_Unused;
};


struct PointLight
{
	float3 Pos;

	float4 Color_LightRange;
};


cbuffer LightBuffer : register(b0)
{
	float4 LightCount_FogRange_ObjectCount_Unused;

	float3 fogColor;
	float pad1;

	float3 CamPos;
	float pad2;

	PointLight lights[MAX_ACTIVE_LIGHTS];
	MatrialDesc materials[MAX_MATERIAL_COUNT];
};


Texture2D shaderTexture[3];
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

	float4 finColor;

	float D = length(CamPos - Pos.xyz);

	float3 V = normalize(CamPos - Pos.xyz);
		float3 N = Normal_Depth.xyz;

		float3 LightCont = float3(0, 0, 0);
		for (int i = 0; i < LightCount_FogRange_ObjectCount_Unused.x; i++)
		{
		float3 S = lights[i].Pos - Pos.xyz;
			float dist = length(S);
			S = normalize(S);
			float end = lights[i].Color_LightRange.w;
			float range = saturate((end - dist) / end);

			float3 R = reflect(-S, N);

				// set ambient contrib.

				float3 am = range*lights[i].Color_LightRange.xyz*0.2;
				LightCont = saturate(LightCont + am);

			// Calculate diffuse Light contrib.
			float3 Ld = range*lights[i].Color_LightRange.xyz*max(dot(S, N), 0);

				// Add diffuse contrib. to color.
				LightCont = saturate(LightCont + Ld);

			// Calculate specular light contrib.
			float3 Ls = range*lights[i].Color_LightRange.xyz*pow(max(dot(R, V), 0), 1.0f);

				// Add specular contrib. to color.
				LightCont = saturate(LightCont + Ls);
		}

	// Calculate draw distance fog
	float fogEnd = 990;
	float fogFactor = 1 - saturate((fogEnd - D) / (fogEnd - LightCount_FogRange_ObjectCount_Unused.y));
	
	finColor = Color * float4(LightCont, 1.0f) + float4(fogColor*fogFactor, 1.0f);



	float3 K = lights[0].Pos - Pos.xyz;
	float di = length(K);
	float en = 50;

	float ran = saturate((en - di) / en);

	return finColor;
}
