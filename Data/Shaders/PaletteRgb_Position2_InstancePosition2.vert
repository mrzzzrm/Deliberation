#version 330 core

in uint ColorIndex;
in vec2 Position;
in vec2 InstancePosition;

flat out uint f_ColorIndex;

void main()
{
    gl_Position = vec4(InstancePosition + Position, 0.0f, 1.0f);
    f_ColorIndex = ColorIndex;
}