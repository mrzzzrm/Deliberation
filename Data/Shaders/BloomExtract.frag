#version 330

uniform sampler2D Input;
uniform float Threshold;

in vec2 f_UV;

out vec3 o_Extracted;

void main()
{
    vec3 inputColor = texture(Input, f_UV).xyz;
    float brightness = length(inputColor);
    vec3 reduced = inputColor - Threshold;

    o_Extracted = max(vec3(0.0f), reduced);
}