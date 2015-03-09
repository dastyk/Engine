cbuffer CamBuffer : register(b0)
{
	float3 CamPos;
};

Texture2D shaderTexture[4];
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Color : COLOR;
	float4 vPos : POSITION;
};

float4 PSMain(PS_IN input) : SV_TARGET
{
	
	//float4 Pos = shaderTexture[2].Sample(SampleType, input.Tex);
	//

	//float3 L = input.vPos.xyz - Pos.xyz;
	//float dist = length(L);
	//float range = saturate((input.vPos.w - dist) / input.vPos.w);

	//float4 Normal_Depth = shaderTexture[0].Sample(SampleType, input.Tex);
	//	float4 Spec_SpecPow = shaderTexture[3].Sample(SampleType, input.Tex);
	//		// Diffuse

	//		float3 ambient = input.Color*0.05;


	//		L /= dist;

	//	float nDotL = saturate(dot(Normal_Depth.xyz, L));
	//	float3 diffuse = nDotL*input.Color*float3(0.7, 0.7, 0.7);

	//		// Specular
	//		float3 V = CamPos - Pos.xyz;
	//		float3 H = normalize(L + V);
	//		float3 specular = pow(saturate(dot(Normal_Depth.xyz, H)), Spec_SpecPow.w)* input.Color* Spec_SpecPow.xyz*nDotL;



	//		return  float4(ambient + diffuse + specular, 1)*range;
			return float4(1, 1, 1, 1);
}
