#version 400

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	fragColor = vec4(1.f, 1.f, 1.f, texture(tex, texCoord).r);
}
