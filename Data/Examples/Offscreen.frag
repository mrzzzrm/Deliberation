#version 330

in vec3 f_Color;
in vec3 f_Normal;

out vec3 o_Color;
out vec3 o_Normal;

void main()
{
	o_Color = f_Color * dot(normalize(f_Normal), vec3(0.1f, 1.0f, 1.0f));
	o_Normal = normalize(f_Normal);
}

