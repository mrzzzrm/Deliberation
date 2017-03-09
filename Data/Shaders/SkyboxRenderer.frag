#version 330

uniform samplerCube Texture;

in vec3 f_UVW;

out vec3 o_Color;

void main()
{
    o_Color = texture(Texture, f_UVW).xyz;// *0.0001 + (f_UVW + 1.0f) / 2.0f;
}
