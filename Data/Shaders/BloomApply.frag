#version 330

uniform sampler2D InputA;
uniform sampler2D InputB;
uniform sampler2D InputC;
uniform sampler2D InputD;

in vec2 f_UV;

out vec3 o_Color;

void main()
{
    vec3 color = vec3(0.0f);

    color += texture(InputA, f_UV).xyz;
    color += texture(InputB, f_UV).xyz;
    color += texture(InputC, f_UV).xyz;
    color += texture(InputD, f_UV).xyz;

    o_Color = color;
}