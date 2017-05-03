#version 330

in vec3 f_Position;
in vec3 f_Color;
in vec3 f_Normal;

out vec3 o_Diffuse;
out vec3 o_Position;
out vec3 o_Normal;

void main()
{
    o_Diffuse = f_Color;
    o_Position = f_Position;
    o_Normal = f_Normal;
}