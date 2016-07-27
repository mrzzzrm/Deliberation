#version 430

uniform mat4 Transform;
uniform mat4 ViewProjection;

in vec3 Position;
in vec3 Color;

out vec3 f_Color;

void main()
{
	gl_Position = ViewProjection * Transform * vec4(Position, 1.0f);
	f_Color = Color;
}

