
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
};

struct PS_OUT
{
	float4 Normal_Depth : SV_TARGET0;
	float4 DiffuseColor : SV_TARGET1;
	float4 Position : SV_TARGET2;
};

PS_OUT PSMain(PS_IN input) : SV_TARGET
{
	PS_OUT output;
	float4 textureColor[3];
	int i = 0;

	// Sample texture
	for (i = 0; i < TCBMSPFR.x; i++)
	{
		textureColor[i] = shaderTexture[i].Sample(SampleType, input.Tex);
	}


	float3 N = normalize(input.Normal);

	float3 norm = float3(0, 1, 0);
	float3 ref = reflect(N, norm);
	float intens = saturate(dot(ref, N));

	output.DiffuseColor = textureColor[1] * intens + textureColor[0] * (1 - intens);
	output.Normal_Depth = float4(N, input.PosH.z);
	output.Position = float4(input.PosH, 1);

	return output;
}

