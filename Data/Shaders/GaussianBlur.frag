#version 330

uniform sampler2D Input;
uniform float Weight[5];
uniform bool Horizontal;

in vec2 f_UV;

out vec3 o_Blurred;

void main()
{
    vec2 tex_offset = 1.0 / textureSize(Input, 0); // gets size of single texel
    vec3 result = texture(Input, f_UV).rgb * Weight[0]; // current fragment's contribution

    if (Horizontal)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(Input, f_UV + vec2(tex_offset.x * i * 1.5f, 0.0)).rgb * Weight[i];
            result += texture(Input, f_UV - vec2(tex_offset.x * i * 1.5f, 0.0)).rgb * Weight[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(Input, f_UV + vec2(0.0, tex_offset.y * i  * 1.5f)).rgb * Weight[i];
            result += texture(Input, f_UV - vec2(0.0, tex_offset.y * i  * 1.5f)).rgb * Weight[i];
        }
    }

    o_Blurred = vec3(result);
}