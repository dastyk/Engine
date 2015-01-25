Texture2D shaderTexture;
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct PS_OUT
{
	float4 Normal_Depth : SV_TARGET0;
	float4 DiffuseColor : SV_TARGET1;
};


PS_OUT PSMain(PS_IN input)
{
	PS_OUT output;
	float4 textureColor;

	// Sample texture
	textureColor = shaderTexture.Sample(SampleType, input.Tex);


	float3 N = normalize(input.Normal);

	output.Normal_Depth = float4(N, input.Pos.z);
		
	output.DiffuseColor = textureColor;


	
	return output;
}

