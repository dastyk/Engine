
#define SHADOW_EPSILON 0.001
#define BUMP_DEPTH 2
#define DETAIL_IT 4

Texture2D shadowMap : register(t0);
Texture2D DetailMap : register(t1);
Texture2D NormalMap : register(t2);
Texture2D shaderTexture0 : register(t3);
Texture2D shaderTexture1 : register(t4);
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
	input.LPos.xy /= input.LPos.w;
	float2 smTex = float2(0.5*input.LPos.x + 0.5f, -0.5f*input.LPos.y + 0.5f);
		
		float sCont = 1;

		float lDepth = shadowMap.Sample(PointSample, smTex).r;
		
		float depth = input.LPos.z / input.LPos.w;
			if (lDepth + SHADOW_EPSILON > depth)
				sCont = 0.3;



	float4 textureColor0, textureColor1;
	int i = 0;





	float3 N = normalize(input.Normal);

	float intens = 1.0f - N.y;

	float depthValue = input.Pos.z / input.Pos.w;
	// Sample texture
	textureColor0 = shaderTexture0.Sample(SampleType, input.Tex);
	textureColor1 = shaderTexture1.Sample(SampleType, input.Tex);


	if (depthValue > 0.01f)
	{
		//float le = lerp(0, 0.025, depthValue);

		////float4 detailColor = DetailMap.Sample(SampleType, input.Tex2*DETAIL_IT);

		float4 detail0 = shaderTexture0.Sample(SampleType, input.Tex2*DETAIL_IT);
			float4 detail1 = shaderTexture1.Sample(SampleType, input.Tex2*DETAIL_IT);

			textureColor0 = textureColor0*(1 - depthValue) + detail0*depthValue*2.0f;
		textureColor1 = textureColor1*(1 - depthValue) + detail1*depthValue*2.0f;


		float3 bumpMap = NormalMap.Sample(SampleType, input.Tex2);

			bumpMap = (bumpMap*((depthValue + 0.4f)*BUMP_DEPTH)) - (((depthValue + 0.4f) *BUMP_DEPTH) / 2.0f);
		//bumpMap *= depthValue*2.0f;
		N = input.Normal + bumpMap.x*input.Tangent + bumpMap.y * input.Binormal;
		N = normalize(N);
	}
	
	output.DiffuseColor = (textureColor0 * intens + textureColor1 * (1 - intens))*sCont;
	output.Normal_Depth = float4(N, input.PosH.z);
	output.Position = float4(input.PosH, 1);
	output.Specular_SpecPower = float4(float3(0.1, 0.1, 0.1), 5);

	//output.DiffuseColor = shaderTexture1.Sample(SampleType, input.Tex);// (textureColor1 * intens + textureColor0 * (1 - intens))*sCont;
	//output.Normal_Depth = float4(input.Normal, input.PosH.z);
	//output.Position = float4(input.PosH, 1);
	//output.Specular_SpecPower = float4(float3(0.1, 0.1, 0.1), 5);

	return output;
}
