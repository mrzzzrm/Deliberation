#version 330

uniform sampler2D Position;
uniform sampler2D Normal;
uniform sampler2D Noise;
uniform vec3 Samples[16];
uniform vec2 NoiseScale;
uniform mat4 Projection;
uniform float Bias;
uniform float Radius;
uniform bool NoiseEnabled;
uniform int NumSamples;

in vec2 f_UV;

out float f_Color;

void main()
{
    vec3 position = texture(Position, f_UV).xyz;
    vec3 normal = texture(Normal, f_UV).xyz;
    vec3 noise = vec3(0.3f, -0.2f, 0.0f);

    if (NoiseEnabled)
    {
        noise = texture(Noise, f_UV * NoiseScale).xyz;
    }

    vec3 tangent = normalize(noise - normal * dot(noise, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 noiseRotation = mat3(tangent, bitangent, normal);

    float occlusion = 0.0f;
    for (int i = 0; i < NumSamples; i++)
    {
        vec3 s = noiseRotation * Samples[i];
        s = position + s * Radius;

        vec4 offset = vec4(s, 1.0f);
        offset = Projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5f + 0.5f;

        float sampleDepth = texture(Position, offset.xy).z;

        float rangeCheck = smoothstep(0.0, 1.0, Radius / abs(s.z - sampleDepth));
        occlusion += (sampleDepth >= s.z + Bias ? 1.0f : 0.0f) * rangeCheck;
    }

    occlusion /= float(NumSamples);

    f_Color = 1.0f - occlusion;
}