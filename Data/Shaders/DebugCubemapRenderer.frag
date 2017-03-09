#version 330

uniform samplerCube Texture;

in vec3 f_UVW;

out vec3 f_Color;

void main()
{
    f_Color = texture(Texture, f_UVW).rgb;
}