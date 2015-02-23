RWTexture2D<float4> output : register(u0);
Texture2D<float4> LightCont : register(t0);
Texture2D<float4> diffuseColor : register(t1);

SamplerState pointSample;

[numthreads(32, 32, 1)] // <-- 32 * 25 = 800 32*20 = 640
void CSMain(uint3 threadID : SV_DispatchThreadID,
		uint groupIndex : SV_GroupIndex)
{
	int3 sampleIndices = int3(threadID.xy, 0);

		float4 Light = LightCont.Load(sampleIndices);
		float4 Color = diffuseColor.Load(sampleIndices);

		output[threadID.xy] = Color*Light;
}