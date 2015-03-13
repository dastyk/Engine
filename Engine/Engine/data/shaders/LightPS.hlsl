
cbuffer PointLight : register(b0)
{
	float3 LPos;

	float4 Color_LightRange;
};

cbuffer CamBuffer : register(b1)
{
	float3 CamPos;
};

Texture2D shaderTexture[4];
SamplerState SampleType;

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};


float4 PSMain(PS_IN input) : SV_TARGET
{

	float4 Pos = shaderTexture[2].Sample(SampleType, input.Tex);
	

	float3 L = LPos - Pos.xyz;
	float dist = length(L);
	float range = saturate((Color_LightRange.w - dist) / Color_LightRange.w);

	if (range > 0.01)
	{
		float4 Normal_Depth = shaderTexture[0].Sample(SampleType, input.Tex);
			float4 Spec_SpecPow = shaderTexture[3].Sample(SampleType, input.Tex);
			// Diffuse

			float3 ambient = Color_LightRange.xyz*0.01; 


			L /= dist;

		float nDotL = saturate(dot(Normal_Depth.xyz, L));
		float3 diffuse = nDotL*Color_LightRange.xyz*float3(0.7, 0.7, 0.7);

			// Specular
			float3 V = CamPos - Pos.xyz;
			float3 H = normalize(L + V);
			float3 specular = pow(saturate(dot(Normal_Depth.xyz, H)), Spec_SpecPow.w)* Color_LightRange.xyz* Spec_SpecPow.xyz*nDotL;



			return float4(ambient + diffuse + specular, 0.5)*range;
	}
	return float4(0, 0, 0, 0);
}
