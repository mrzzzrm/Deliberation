#version 330

uniform usampler2D Texture;

in vec2 f_UV;
in vec4 f_Color;

out vec4 o_Color;
out float gl_FragDepth;

void main()
{
    o_Color = f_Color * vec4(texture(Texture, f_UV.st)) / vec4(255.0f);
    gl_FragDepth = 0.0f;
}