#version 330

uniform samplerCube Texture;

in vec3 f_UVW;

out vec3 f_Color;

void main()
{
    vec3 uvw = f_UVW;

     uvw.x = -uvw.x;

    uvw.y = -uvw.y;
   // uvw.z = -uvw.z;

    f_Color = texture(Texture, uvw).rgb;
}