#version 330

in vec3 f_Color;

out vec3 o_Color0;
out vec3 o_Color1;

void main()
{
	o_Color0 = f_Color;
	o_Color1 = vec3(1.0f) - f_Color;
}

