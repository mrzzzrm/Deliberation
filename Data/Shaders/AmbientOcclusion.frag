#version 330

uniform vec3 		Kernel[32];
uniform sampler2D 	NormalDepth;
uniform sampler2D 	Rotation;
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
	vec3 origin = f_ViewRay * depth;
	
	vec3 rvec = texture(Rotation, f_UV * RotationCoordScale).xyz;
	vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 tbn = mat3(tangent, bitangent, normal);
	
	float occlusion = 0.0f;
	
	for (int s = 0; s < 32; s++)
	{
		vec3 sample = tbn * Kernel[s];
		sample = sample * SampleRadius + origin;
		
		vec4 offset = vec4(sample, 1.0f);
		offset = Projection * offset;
		offset.xy /= offset.w;
		offset.xy = offset.xy * 0.5f + 0.5f;
		
		float sampleDepth = texture(NormalDepth, offset.xy).w;
		
		float rangeCheck = abs(origin.z - (f_ViewRay * sampleDepth).z) < SampleRadius ? 1.0f : 0.0f;
		occlusion += ((f_ViewRay * sampleDepth).z >= sample.z ? 1.0f : 0.0f) * rangeCheck;
	}
	
	if (depth > 0.99f)
	{
		o_Occlusion = vec3(0.0f);
	}
	else
	{
		o_Occlusion = vec3(1 - occlusion / 32.0f);		
	}
}

