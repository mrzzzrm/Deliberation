#version 400

uniform vec3 Color;
uniform sampler2D Texture;

in vec2 f_UV;

out vec4 o_Color;

void main()
{
    vec3 rgb = Color;
    float a = texture(Texture, f_UV).a;

    if (a == 0.0f) discard;

    o_Color = texture(Texture, f_UV) + vec4(Color, 0.0f) * 0.00001f;//vec4(rgb, a);
}

