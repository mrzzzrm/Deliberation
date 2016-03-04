#version 330

uniform sampler2D Occlusion;
uniform sampler2D Color;

in vec2 f_UV;

out vec3 o_Output;

void main()
{
	o_Output = texture(Color, f_UV).xyz * texture(Occlusion, f_UV).x;
}


