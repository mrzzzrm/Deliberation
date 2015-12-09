#version 430

layout(location = 5) uniform sampler2D Texture;

in vec2 f_UV;

out vec3 o_Color;

void main()
{
	o_Color = texture(Texture, f_UV).rgb;
}

