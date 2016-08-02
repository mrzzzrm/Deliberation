#version 330

uniform vec3 LightDirection;

in vec3 f_Normal;
in vec3 f_VS_Position;

out vec3 f_Color;

void main()
{
    vec3 normal = normalize(f_Normal);
    vec3 view = normalize(-f_VS_Position);
    vec3 light = normalize(-LightDirection);
    vec3 color = vec3(0.6f, 0.5f, 0.3f);

    vec3 nlight = clamp(color * dot(normal, light), 0.0f, 1.0f);
    vec3 nview = clamp(color * dot(normal, view), 0.0f, 1.0f);

	f_Color = color * 0.2 + color * nlight;
}

