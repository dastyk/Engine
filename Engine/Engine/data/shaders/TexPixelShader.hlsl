
Texture2D shaderTexture;
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	uint Id : BLENDINDICES;
};


float4 PSMain(PS_IN input) : SV_TARGET
{
	float4 textureColor;

	textureColor = shaderTexture.Sample(SampleType, input.Tex);

	float4 color = saturate(textureColor);

		//if (color.r > 0.75 && color.g > 0.75 && color.b > 0.75)
		//{
		//discard;
		//}
	return color;
}