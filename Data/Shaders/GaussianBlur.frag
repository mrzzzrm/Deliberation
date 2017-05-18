#version 330

uniform Config
{
    uniform float Weights[16];
    uniform float Offsets[16];
    uniform int NumSamples;
};

uniform sampler2D Input;
uniform bool Horizontal;

in vec2 f_UV;

out vec3 o_Blurred;

void main()
{
    vec2 fragSize = 1.0 / textureSize(Input, 0); // gets size of single texel

    vec3 result = texture(Input, f_UV).rgb * Weights[0];

    if (Horizontal)
    {
        for (int i = 1; i < NumSamples; i++)
        {
            result += texture(Input, f_UV + vec2(fragSize.x * Offsets[i], 0.0)).rgb * Weights[i];
            result += texture(Input, f_UV - vec2(fragSize.x * Offsets[i], 0.0)).rgb * Weights[i];
        }
    }
    else
    {
        for (int i = 1; i < NumSamples; i++)
        {
            result += texture(Input, f_UV + vec2(0.0, fragSize.y * Offsets[i])).rgb * Weights[i];
            result += texture(Input, f_UV - vec2(0.0, fragSize.y * Offsets[i])).rgb * Weights[i];
        }
    }

    o_Blurred = vec3(result);
}