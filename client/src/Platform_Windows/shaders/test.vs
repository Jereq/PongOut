//GLOBALS

//DESCS

struct VSIN
{
	float2 center		: ANCHOR;
	float2 dimensions	: DIMENSIONS;
};

struct PSIN
{
	float4 p		: SV_POSITION;
};

//SHADER
/*PSIN VShader(VSIN input)
{
	PSIN output;
	output.p = float4(input.center.x, input.center.y, input.dimensions.x, input.dimensions.y);

	return output;
}*/

VSIN VShader(VSIN input)
{
	VSIN output;
	output.center = input.center;// = float4(input.center.x, input.center.y, 1,1);
	output.dimensions = input.dimensions;

	return output;
}