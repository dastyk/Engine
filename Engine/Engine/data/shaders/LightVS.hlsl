
struct VS_IN
{
	float4 Pos : POSITION;
	float3 Color : COLOR;
};


VS_IN VSMain(VS_IN input)
{
	return input;
}