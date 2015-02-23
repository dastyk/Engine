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

struct PS_OUT
{
	float4 Normal_Depth : SV_TARGET0;
	float4 DiffuseColor : SV_TARGET1;
	float4 Position : SV_TARGET2;
	float4 Specular_SpecPower : SV_TARGET3;
};

PS_OUT PSMain(PS_IN input)
{
	PS_OUT output;
	float4 textureColor[3];
	int i = 0;

	// Sample texture
	for (i = 0; i < 2; i++)
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
	output.Specular_SpecPower = float4(float3(0.1,0.1,0.1), 5);
	return output;
}

