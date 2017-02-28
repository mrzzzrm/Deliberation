#version 430

uniform mat4 T;

in vec2 Position;
in vec4 Color;

out vec4 f_Color;
out vec2 f_UV;

void main()
{
	gl_Position = T * vec4(Position, 0.5f, 1.0f);
	f_Color = Color;
	f_UV = Position;
}

