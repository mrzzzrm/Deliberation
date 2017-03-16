#version 400

uniform vec3 Color;
uniform sampler2D Texture;

in vec2 f_UV;

out vec3 o_Color;

void main()
{
    o_Color = vec3(1.0) + Color * texture(Texture, f_UV).a;
}

