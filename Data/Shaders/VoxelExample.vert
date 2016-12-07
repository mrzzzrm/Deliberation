#version 330

uniform mat4 ViewProjection;

in vec3 Position;
in vec3 Normal;
in vec3 Color;

out vec3 f_Normal;
out vec3 f_Color;

void main()
{
    gl_Position = ViewProjection * vec4(Position, 1.0f);
    f_Normal = Normal;
    f_Color = Color;
}