

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};


float4 PSMain(VS_OUT input) : SV_TARGET
{
	return input.Color;
}