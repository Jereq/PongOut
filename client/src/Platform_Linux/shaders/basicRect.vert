#version 400

in vec3 vertexPosition;
in vec2 vertexSize;

out vec3 position;
out vec2 size;

uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * vec4(vertexPosition, 1.f);
	position = gl_Position;
	size = vertexSize;
}
