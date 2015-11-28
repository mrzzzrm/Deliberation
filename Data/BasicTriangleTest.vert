#version 430

uniform mat4 T;

in vec2 Position;
in vec3 Color;

out vec3 f_Color;

void main()
{
	gl_Position = T * vec4(Position, 0.5f, 1.0f);
	f_Color = Color;
}

