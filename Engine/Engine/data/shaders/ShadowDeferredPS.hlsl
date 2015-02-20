
#define SHADOW_EPSILON 0.001

Texture2D shadowMap : register(t0);
Texture2D shaderTexture[3] : register(t1);
SamplerState SampleType : register(s0);
SamplerState PointSample : register(s1);

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 PosH : POSITION0;
	float2 Tex2 : TEXCOORD1;
	float4 LPos : POSITION1;
};
struct PS_OUT
{
	float4 Normal_Depth : SV_TARGET0;
	float4 DiffuseColor : SV_TARGET1;
	float4 Position : SV_TARGET2;
};


PS_OUT PSMain(PS_IN input)
{
	PS_OUT output;
	input.LPos.xy /= input.LPos.w;
	float2 smTex = float2(0.5*input.LPos.x + 0.5f, -0.5f*input.LPos.y + 0.5f);
		
		float sCont = 1;

		float lDepth = shadowMap.Sample(PointSample, smTex).r;
		
		float depth = input.LPos.z / input.LPos.w;
			if (lDepth + SHADOW_EPSILON > depth)
				sCont = 0.3;



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

	output.DiffuseColor = (textureColor[1] * intens + textureColor[0] * (1 - intens))*sCont ;
	output.Normal_Depth = float4(N, input.PosH.z);
	output.Position = float4(input.PosH, 1);

	return output;
}
