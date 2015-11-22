#version 330

in vec3 f_Color;

out vec3 o_Color;

void main()
{
	o_Color = vec3(1.0f, 0.0f, 0.0f) + f_Color;
}

