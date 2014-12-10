
cbuffer LightBuffer : register(cb0)
{
	float3 ambientColor;
	int textureCount;
	float3 ambientReflection;

	float3 diffDir;
	float3 diffColor;
	float3 diffReflection;

	float3 specDir;
	float3 specColor;
	float3 specReflection;
	float specShinyPower;

	float3 fogColor;
	float fogRange;
};

Texture2D shaderTexture[2];
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 PosH : POSITION;
};

struct PS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

float4 PSMain(PS_IN input) : SV_TARGET
{
	float4 textureColor[2];
	float  D = length(input.PosH);
	int i = 0;
	// Sample texture
	for (i = 0; i < textureCount; i++)
	{
		textureColor[i] = shaderTexture[i].Sample(SampleType, input.Tex);
	}

	// set ambient contrib.
	float3 color = ambientReflection*ambientColor;

	float3 V = normalize(-input.PosH);
	float3 N = normalize(input.Normal);
	float3 S = normalize(-specDir);
	float3 R = reflect(-S, N);
	// Calculate diffuse Light contrib.
	float3 Ld = diffReflection*diffColor*max(dot(-diffDir, N), 0);

	// Add diffuse contrib. to color.
	color = saturate(color + Ld);

	// Calculate specular light contrib.
	float3 Ls = specReflection*specColor*pow(max(dot(R, V),0), specShinyPower);

	// Add specular contrib. to color.
	color = saturate(color + Ls);

	// Calculate draw distance fog
	float fogEnd = 990;
	float fogFactor = 1 - saturate((fogEnd - D) / (fogEnd - fogRange));

	// Factor in the fog to the final light
	color = color*(1 - fogFactor);

	// Multiply orig texture with final lighting and att fogColor*fogFactor
	float4 finColor = float4(1, 1, 1, 1);
	for (i = 0; i < textureCount; i++)
	{
		finColor = finColor * textureColor[i] * 2.0f;
	}

	finColor = saturate(finColor);

	finColor = finColor * float4(color, 1) + float4(fogColor*fogFactor, 1);


	return finColor;
}

