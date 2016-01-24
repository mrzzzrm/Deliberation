#version 330

uniform mat4 viewProjection;

in vec3 Position;

void main()
{
    gl_Position = viewProjection * vec4(Position, 1.0f);
}

