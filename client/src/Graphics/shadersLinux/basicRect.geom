#version 400

layout (points) in;
layout (triangle_strip) out;

in vec3 position[];
in vec2 size[];

out vec2 texCoord;

uniform mat4 projectionMatrix;

void main()
{
	vec2 topRight = position[0].xy + size[0] * 0.5f;
	vec2 lowerLeft = position[0].xy - size[0] * 0.5f;
	
	gl_Position = vec4(lowerLeft.x, topRight.y, position[0].z, 1.f);
	texCoord = vec2(0.f, 1.f);
	EmitVertex();
	
	gl_Position = vec4(lowerLeft.x, lowerLeft.y, position[0].z, 1.f);
	texCoord = vec2(0.f, 0.f);
	EmitVertex();
	
	gl_Position = vec4(topRight.x, topRight.y, position[0].z, 1.f);
	texCoord = vec2(1.f, 1.f);
	EmitVertex();
	
	gl_Position = vec4(topRight.x, lowerLeft.y, position[0].z, 1.f);
	texCoord = vec2(1.f, 0.f);
	EmitVertex();
	
	EndPrimitive();
}
