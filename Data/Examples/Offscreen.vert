#version 330

in vec2 Position;
in vec3 Color;

out vec3 f_Color;

void main()
{
	gl_Position = vec4(Position, 0.0f, 1.0f);
	f_Color = Color;
}

