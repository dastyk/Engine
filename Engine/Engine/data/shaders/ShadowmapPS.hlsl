
struct PS_IN
{
	float4 Pos : SV_POSITION;
};
float4 PSMain(PS_IN input) : SV_TARGET
{
	return float4(input.Pos.z, 0, 0, 0);
}

