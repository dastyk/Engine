cbuffer MaterialBuffer
{

	float3 Ambient;
	float3 Diffuse;
	float3 Specular;
	float3 Reflectivity;

	float4 SpecPower_AlphaClip_Unused_Unused;

};


Texture2D shaderTexture;
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float4 BlendWeights : BLENDWEIGHT;
	uint4 BlendIndices : BLENDINDICES0;
	float3 PosH : POSITION;
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
	float4 textureColor;
	// Sample texture
	textureColor = shaderTexture.Sample(SampleType, input.Tex);

	float3 N = normalize(input.Normal);

	output.Normal_Depth = float4(N, 0);
		
	output.DiffuseColor = textureColor;

	output.Position = float4(input.PosH,1);
	output.Specular_SpecPower = float4(Specular, SpecPower_AlphaClip_Unused_Unused.x);
	return output;
}

