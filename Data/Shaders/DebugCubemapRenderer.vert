#version 330

uniform mat4 ViewProjection;
uniform mat4 Transform;

in vec3 Position;

out vec3 f_UVW;

void main()
{
    gl_Position = ViewProjection * (Transform * vec4(Position, 1.0f));
    f_UVW = Position;
}