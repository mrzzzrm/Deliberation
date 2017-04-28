#version 330

uniform sampler2D Hdr;
uniform sampler2D Depth;

in vec2 f_UV;

out vec3 o_Color;

void main()
{
    vec3 hdr = texture(Hdr, f_UV).rgb;
    o_Color = hdr / (hdr * 0.1f + 1.0f);
    gl_FragDepth = texture(Depth, f_UV).r;
}