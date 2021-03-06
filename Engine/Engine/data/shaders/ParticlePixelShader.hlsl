struct PS_IN
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
	float2 Tex : TEXCOORD0;
};

Texture2D shaderTexture;
SamplerState SampleType;

float4 PSMain(PS_IN input) : SV_TARGET
{


	float4 textureColor;

	// Sample texture
	textureColor = shaderTexture.Sample(SampleType, input.Tex);

	/*if (textureColor.x > 0.7 && textureColor.y > 0.7 && textureColor.z > 0.7)
		discard;*/

	return float4((textureColor.xyz + input.Color), 1);
}