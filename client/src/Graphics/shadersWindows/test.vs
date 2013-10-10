//GLOBALS


//DESCS

struct VSIN
{
	float3 center		: ANCHOR;
	float2 dimensions	: DIMENSIONS;
};

struct PSIN
{
	float4 p		: SV_POSITION;
	float2 t		: TEXCOORD0;
};

VSIN VShader(VSIN input)
{
	VSIN output;
	output.center 		= input.center;
	output.dimensions 	= input.dimensions;

	return output;
}