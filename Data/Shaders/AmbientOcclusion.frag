#version 330

uniform vec3 		Kernel[32];
uniform sampler2D 	NormalDepth;
uniform sampler2D 	Rotation;
uniform sampler2D 	PositionVS;
uniform vec2 		RotationCoordScale;
uniform float 		SampleRadius;
uniform mat4		Projection;

in vec2 f_UV;
in vec3 f_ViewRay;

out vec3 o_Occlusion;

void main()
{
	vec4 normalDepth = texture(NormalDepth, f_UV);
	float depth = normalDepth.w;
	vec3 normal = normalDepth.xyz;
	vec3 originVS = texture(PositionVS, f_UV).xyz * 1.0f + 0.0f * f_ViewRay * depth;
	
	vec3 rvec = texture(Rotation, f_UV * RotationCoordScale).xyz;
	vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 tbn = mat3(tangent, bitangent, normal);
	
	vec3 occlusion = vec3(0.0f);
	
	float count = 1.0f;
		
	for (int s = 0; s < 32; s++)
	{
		vec3 sample = tbn * Kernel[s];
		sample = sample * SampleRadius + originVS;
		
		vec4 offset = vec4(sample, 1.0f);
		offset = Projection * offset;
		offset.xy /= offset.w;
		offset.xy = offset.xy * 0.5f + 0.5f;
		
		float sampleDepth = texture(NormalDepth, offset.xy).w;
		float sampleZVS = texture(PositionVS, offset.xy).z * 1.0f + (f_ViewRay * sampleDepth).z * 0.0f;
		
		float oos = (1.0f - step(1.0f, offset.x)) * step(0.0f, offset.x);
		oos = oos * (1.0f - step(1.0f, offset.y)) * step(0.0f, offset.y);
		
		float rangeCheck = (abs(originVS.z - sampleZVS) < SampleRadius * 1.1f ? 1.0f : 0.0f) * oos;
		count += rangeCheck;
		occlusion += vec3(sampleZVS >= sample.z ? 1.0f : 0.0f) * rangeCheck;
	}
	
	if (depth > 0.99f || count == 0.0f)
	{
		o_Occlusion = vec3(1.0f);
	}
	else
	{		
		o_Occlusion = vec3(1 - occlusion / max(count, 1));		
	}
}

