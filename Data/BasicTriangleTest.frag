#version 430

uniform float Fade;

in vec4 f_Color;
in vec2 f_UV;

out vec4 o_Color;

void main()
{
    float intensity = max(1.0f - length(f_UV), 0.0f);

    o_Color.rgb = vec3(1.0f, 1.0f, 1.0f);
    o_Color.a = intensity * Fade;
}

