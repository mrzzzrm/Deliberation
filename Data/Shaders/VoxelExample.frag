#version 330

in vec3 f_Normal;
in vec3 f_Color;

out vec3 o_Color;

void main()
{
    vec3 light = normalize(vec3(0.1f, 1.0f, 0.2f));
	o_Color = f_Color * 0.6 + f_Color * dot(f_Normal, light) * 0.4f;
}