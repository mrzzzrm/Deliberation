#version 330

uniform mat4 transform;

in vec3 Position;
in vec3 Normal;

out vec3 f_Color;
out vec3 f_Normal;

void main()
{
	gl_Position = transform * vec4(Position, 1.0f);
	f_Color = vec3(0.8, 0.4, 0.4);
	f_Normal = Normal;
}

