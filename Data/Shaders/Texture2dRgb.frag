#version 330

uniform sampler2D Input;

in vec2 f_UV;

out vec3 o_Color;

void main()
{
    o_Color = texture(Input, f_UV).rgb;
}