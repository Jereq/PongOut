
//--- GLOBALS ---//
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//--- DESCS ---//
struct VS
{
	float4 position	: POSITION;
};

struct PS
{
	float4 position	: SV_POSITION;
};

// VSHADER
PS TestVShader(VS input)
{
	PS output;
	
	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	return output;
};