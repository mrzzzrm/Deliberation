#version 330 core

uniform samplerBuffer Palette;

flat in uint f_ColorIndex;

out vec3 o_Rgb;

void main()
{
    o_Rgb = texelFetch(Palette, int(f_ColorIndex)).xyz;
}