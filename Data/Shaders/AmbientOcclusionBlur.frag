#version 330

uniform sampler2D 	Occlusion;
uniform sampler2D 	NormalDepth;
uniform vec2		TexelSize;
uniform int			KernelSize;

in vec2 f_UV;

out vec3 o_BlurredOcclusion;

#define COMP_DEPTH 0.01
#define COMP_NORMAL 0.5

void main()
{
	vec4 normalDepth = texture(NormalDepth, f_UV);
	vec3 normal = normalDepth.xyz;
	float depth = normalDepth.w;

    vec3 blurredOcclusion = vec3(0.0);
    int count = 0;
        
    vec2 TexelOffset = -(KernelSize / 2.0f) * TexelSize;
    
    for (int x = 0; x < KernelSize; ++x)
    {
		for (int y = 0; y < KernelSize; ++y)
		{
			vec2 sampleUV = f_UV + vec2(x, y) * TexelSize + TexelOffset;

			vec4 sampleNormalDepth = texture(NormalDepth, sampleUV);
			vec3 sampleNormal = sampleNormalDepth.xyz;
			float sampleDepth = sampleNormalDepth.w;

			float depthTest = abs(sampleDepth - depth) < COMP_DEPTH ? 1.0 : 0.0;
			float normalTest = dot(normal, sampleNormal.xyz) > COMP_NORMAL ? 1.0 : 0.0;
			blurredOcclusion += texture(Occlusion, sampleUV).xyz * depthTest * normalTest;
			count += int(depthTest * normalTest);
		}
    }
    
    o_BlurredOcclusion = blurredOcclusion / count;
}


