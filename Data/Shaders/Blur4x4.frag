#version 330

uniform sampler2D Input;

in vec2 f_UV;

out float f_Color;


void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(Input, 0));

    float result = 0.0;

    for (int x = -2; x < 2; ++x)
    {
        for (int y = -2; y < 2; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(Input, f_UV + offset).r;
        }
    }

    f_Color = result / 16.0f;
}