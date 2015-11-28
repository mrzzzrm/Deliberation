#version 430

layout(location = 5) uniform vec3 BaseColor;

in vec3 f_Color;

out vec3 o_Color;

void main()
{
	o_Color = BaseColor + f_Color;
}

