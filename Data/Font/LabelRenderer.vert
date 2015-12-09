#version 400

uniform mat4 Transform;

in vec2 Position;

out vec2 f_UV;

void main()
{
    gl_Position = Transform * vec4(Position * 2.0f, 0.0f, 1.0f);
    f_UV = Position * 2.0f * vec2(0.5f) + vec2(0.5f);
}

