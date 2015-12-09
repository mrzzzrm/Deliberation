#version 430

in vec2 Position;
in vec2 UV;

out vec2 f_UV;

void main()
{
	gl_Position = vec4(Position, 0.5f, 1.0f);
	f_UV = UV;
}

