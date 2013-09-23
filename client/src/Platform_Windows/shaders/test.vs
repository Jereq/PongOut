//GLOBALS

//DESCS

struct VSIN
{
	float2 topLeft		: ANCHOR;
	float2 dimensions	: DIMENSIONS;
};

struct PSIN
{
	float4 p		: SV_POSITION;
};

//SHADER
PSIN VShader(VSIN input)
{
	PSIN output;
	float movX = -0.5;
	float movY = 0.5;
	output.p = float4(input.topLeft.x + movX, input.topLeft.y + movY, 1, 1);

	return output;
}