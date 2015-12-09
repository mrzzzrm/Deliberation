#version 430

uniform sampler2D Texture;

in vec2 f_UV;

out vec3 o_Color;

void main()
{
	vec4 pixel = texture(Texture, f_UV);
	o_Color = pixel.rgb * pixel.a;
}

