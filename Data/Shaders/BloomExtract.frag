#version 330

uniform sampler2D Input;
uniform vec3 Threshold;

in vec2 f_UV;

out vec3 o_Extracted;

void main()
{
    vec3 inputColor = texture(Input, f_UV).xyz;
    float brightness = dot(inputColor, normalize(Threshold)) / 1.4f;
    vec3 reduced = inputColor - (inputColor / max(1.0f, brightness));

    o_Extracted = max(vec3(0.0f), reduced);
}