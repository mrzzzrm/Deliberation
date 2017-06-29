#version 330

uniform samplerCube Texture;

in vec3 f_UVW;

out vec3 o_Hdr;

void main()
{
    o_Hdr = texture(Texture, f_UVW).xyz;
    gl_FragDepth = 1.0f;
}
