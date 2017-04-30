#version 330

uniform sampler2D Position;
uniform sampler2D Diffuse;
uniform sampler2D Normal;

in vec3 f_LightPosition;
in vec3 f_Intensity;
//in vec2 f_UV;
in float f_Radius;

out vec3 f_Color;

const float RadiusMin = 0.01f;

void main()
{
    vec2 f_UV = gl_FragCoord.xy / textureSize(Diffuse, 0);

    vec3 surfacePosition = texture(Position, f_UV).xyz;
    vec3 surfaceNormal = texture(Normal, f_UV).xyz;
    vec3 surfaceDiffuse = texture(Diffuse, f_UV).xyz;

    vec3 lightDelta = f_LightPosition - surfacePosition;
    float distance = length(lightDelta);
    float falloff = 1.0f / (distance * distance);//(f_Radius - distance) / (f_Radius - RadiusMin);
    if (distance > f_Radius) falloff = 0.0f;
    if (distance < RadiusMin) falloff = 1.0f;

    vec3 lightDir = normalize(lightDelta);
    float surfaceNormalLightDot = dot(surfaceNormal, lightDir);

    float backside = step(0.0f, surfaceNormalLightDot);

    f_Color = falloff * backside * (surfaceNormalLightDot) * f_Intensity * surfaceDiffuse;
}