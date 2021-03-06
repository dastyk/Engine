
cbuffer LightBuffer : register(b0)
{
	float4 TCBMSPFR;

	float3 ambientColor;
	float3 ambientReflection;

	float3 diffDir;
	float3 diffColor;
	float3 diffReflection;

	float3 specDir;
	float3 specColor;
	float3 specReflection;


	float3 fogColor;

};

Texture2D shaderTexture[3] : register(t0);
Texture2D blendMap : register(t3);
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 PosH : POSITION;
	float2 Tex2 : TEXCOORD1;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
};

float4 PSMain(PS_IN input) : SV_TARGET
{
	float4 textureColor[3];
	float  D = length(input.PosH);
	int i = 0;

	// Sample texture
	for (i = 0; i < TCBMSPFR.x; i++)
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
	float3 Ls = specReflection*specColor*pow(max(dot(R, V), 0), TCBMSPFR.z);

	// Add specular contrib. to color.
	color = saturate(color + Ls);

	// Calculate draw distance fog
	float fogEnd = 490;
	float fogFactor = 1 - saturate((fogEnd - D) / (fogEnd - TCBMSPFR.w));

	// Factor in the fog to the final light
	color = color*(1 - fogFactor);

	// Multiply orig texture with final lighting and att fogColor*fogFactor
	float4 finColor = float4(0, 0, 0, 1);

	if (TCBMSPFR.y)
	{
		float4 blendMapColor = blendMap.Sample(SampleType, input.Tex2);


			float4 w1 = blendMapColor.x / (blendMapColor.x + blendMapColor.y + blendMapColor.z + 0.000001);
			float4 w2 = blendMapColor.y / (blendMapColor.x + blendMapColor.y + blendMapColor.z + 0.000001);
			float4 w3 = blendMapColor.y / (blendMapColor.x + blendMapColor.y + blendMapColor.z + 0.000001);

			float3 norm = float3(0, 1, 0);
			float3 ref = reflect(N, norm);
			float intens = saturate(dot(ref, N));

			//finColor = textureColor[0] * w1  + textureColor[1] * w2;// +textureColor[2] * w3; float4(1, 0, 0, 1);
		finColor = textureColor[1] * intens  + textureColor[0] * (1 - intens);
	}
	else
	{

		finColor = float4(1, 1, 1, 1);
		for (int i = 0; i < TCBMSPFR.x; i++)
		{
			finColor *= textureColor[i];
		}

	}
	finColor = saturate(finColor);

	finColor = finColor * float4(color, 1) + float4(fogColor*fogFactor, 1);


	return finColor;
}

