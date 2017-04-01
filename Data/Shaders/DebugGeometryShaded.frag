#version 330

uniform vec3 Color;

in vec3 f_Normal;

out vec3 o_Color;

void main()
{
    vec3 light = normalize(vec3(0.1f, 1.0f, 0.2f));
	o_Color = Color * 0.6 + Color * max(0.0f, dot(normalize(f_Normal), light)) * 0.4f;
}

