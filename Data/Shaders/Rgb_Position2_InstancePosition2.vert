#version 330

in vec3 Rgb;
in vec2 Position;
in vec2 InstancePosition;

out vec3 f_Rgb;

void main()
{
    gl_Position = vec4(InstancePosition + Position, 0.0f, 1.0f);
    f_Rgb = Rgb;
}