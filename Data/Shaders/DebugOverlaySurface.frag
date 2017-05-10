#version 330

uniform sampler2D Input;

in vec2 f_UV;

out vec3 o_Color;

void main()
{
    vec3 i = texture(Input, f_UV).xyz;
    o_Color = i / (i + 1.0f);
}