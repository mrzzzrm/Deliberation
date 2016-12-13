#version 330

uniform mat4 ViewProjection;
uniform mat4 Transform;

in vec3 Position;
in vec3 Normal;
in vec3 Color;

out vec3 f_Normal;
out vec3 f_Color;

void main()
{
    gl_Position = ViewProjection * vec4(Position, 1.0f);
	f_Normal = (Transform * vec4(Normal, 0.0f)).xyz;
    f_Color = Color;
}