#define MAX_MATERIAL_COUNT 10

Texture2D shaderTexture[MAX_MATERIAL_COUNT];
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float4 BlendWeights : BLENDWEIGHT;
	uint4 BlendIndices : BLENDINDICES0;
	uint Id : BLENDINDICES1;
	float3 PosH : POSITION;
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


	float3 N = normalize(input.Normal);

	output.Normal_Depth = float4(N, 0);
		
	output.DiffuseColor = textureColor;

	output.Position = float4(input.PosH,1);
	
	return output;
}

