Texture2D shaderTexture0 : register(t0);
Texture2D shaderTexture1 : register(t1);
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};


float4 PSMain(PS_IN input) : SV_TARGET
{
	//float test = 1;
	float4 Light = shaderTexture0.Sample(SampleType, input.Tex);
	float4 Color = shaderTexture1.Sample(SampleType, input.Tex);
	
	//Color = float4(test, test, test, 1);



	return Color*Light;
}
