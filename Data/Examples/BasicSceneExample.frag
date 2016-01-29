#version 330

in vec3 f_Normal;

out vec3 f_Color;

void main()
{
	f_Color = vec3(0.6f, 0.5f, 0.3f) * dot(normalize(f_Normal), vec3(0.1f, 1.0f, 1.0f));
}

