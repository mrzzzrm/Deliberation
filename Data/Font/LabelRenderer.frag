#version 400

uniform vec3 Color;
uniform sampler2D Texture;

in vec2 f_UV;

out vec3 o_Color;

void main()
{
    o_Color = Color * texture(Texture, f_UV).a;
}

