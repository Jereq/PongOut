#version 400

in vec3 vertexPosition;
in vec2 vertexTexCoord;

out vec2 texCoord;

uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * vec4(vertexPosition, 1.f);
	
	texCoord = vertexTexCoord;
}
