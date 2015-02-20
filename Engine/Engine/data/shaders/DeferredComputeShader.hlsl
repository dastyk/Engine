RWTexture2D<float4> output : register(u0);
[numthreads(32, 32, 1)] // <-- 32 * 25 = 800 32*20 = 640
void CSMain(uint3 threadID : SV_DispatchThreadID)
{
	output[threadID.xy] = float4(1, 0, 0, 1);
}