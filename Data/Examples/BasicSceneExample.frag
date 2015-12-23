#version 330

in vec3 f_Normal;

out vec3 f_Color;

void main()
{
	f_Color = vec3(0.8f, 0.7f, 0.4f) * dot(normalize(f_Normal), vec3(0.1f, 1.0f, 1.0f));
}

