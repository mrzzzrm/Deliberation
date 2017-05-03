#version 330

uniform sampler2D Ssao;
uniform sampler2D Diffuse;

in vec2 f_UV;

out vec3 o_Hdr;

void main()
{
    o_Hdr = texture(Ssao, f_UV).r * texture(Diffuse, f_UV).rgb;
}
