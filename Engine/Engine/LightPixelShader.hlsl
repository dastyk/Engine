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

Texture2D shaderTexture[MAX_MATERIAL_COUNT];
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 PosH : POSITION;
	uint Id : BLENDINDICES;
};

float4 PSMain(PS_IN input) : SV_TARGET
{
	float4 textureColor;
	

	// Sample texture
	switch (input.Id)
	{
	case 0:
		textureColor = shaderTexture[0].Sample(SampleType, input.Tex);
		break;
	case 1:
		textureColor = shaderTexture[1].Sample(SampleType, input.Tex);
		break;
	case 2:
		textureColor = shaderTexture[2].Sample(SampleType, input.Tex);
		break;
	case 3:
		textureColor = shaderTexture[3].Sample(SampleType, input.Tex);
		break;
	case 4:
		textureColor = shaderTexture[4].Sample(SampleType, input.Tex);
		break;
	case 5:
		textureColor = shaderTexture[5].Sample(SampleType, input.Tex);
		break;
	case 6:
		textureColor = shaderTexture[6].Sample(SampleType, input.Tex);
		break;
	case 7:
		textureColor = shaderTexture[7].Sample(SampleType, input.Tex);
		break;
	case 8:
		textureColor = shaderTexture[8].Sample(SampleType, input.Tex);
		break;
	case 9:
		textureColor = shaderTexture[9].Sample(SampleType, input.Tex);
		break;
	}

	float3 V = CamPos - input.PosH;
		float  D = length(V);
	V = normalize(V);
		float3 N = normalize(input.Normal);

	float3 color = float3(0.5, 0.5, 0.5);

		for (uint i = 0; i < LightCount_FogRange_ObjectCount_Unused.x; i++)
		{

		float3 S = lights[i].Pos - input.PosH;
			float dist = length(S);
		S = normalize(S);
		float end = lights[i].Color_LightRange.w;

		float range = saturate((end - dist) / end);

		float3 R = reflect(-S, N);

		// set ambient contrib.
		color *= materials[input.Id].Ambient*range*lights[i].Color_LightRange.xyz;

		

		

			// Calculate diffuse Light contrib.
		float3 Ld = range*materials[input.Id].Diffuse* lights[i].Color_LightRange.xyz*max(dot(S, N), 0);

			// Add diffuse contrib. to color.
			color = saturate(color + Ld);

		// Calculate specular light contrib.
		float3 Ls = range*materials[input.Id].Specular*lights[i].Color_LightRange.xyz*pow(max(dot(R, V), 0), materials[input.Id].SpecPower_AlphaClip_Unused_Unused.x);

			// Add specular contrib. to color.
			color = saturate(color + Ls);
		}
	// Calculate draw distance fog
	float fogEnd = 990;
	float fogFactor = 1 - saturate((fogEnd - D) / (fogEnd - LightCount_FogRange_ObjectCount_Unused.y));

	// Factor in the fog to the final light
	color = color*(1 - fogFactor);

	// Multiply orig texture with final lighting and att fogColor*fogFactor
	textureColor = textureColor * float4(color, 1) + float4(fogColor*fogFactor, 1);


	return textureColor;
}

