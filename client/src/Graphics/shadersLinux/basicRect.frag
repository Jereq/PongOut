#version 400

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	vec4 color = texture(tex, texCoord);
	if (color.a <= 0.0f)
	{
		discard;
	}
	else
	{
		fragColor = color;
	}
}
