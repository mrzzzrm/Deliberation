#version 330

in vec4 f_NormalDepth;
in vec3 f_PositionVS;

out vec3 o_Color;
out vec4 o_NormalDepth;
out vec3 o_PositionVS;

void main()
{
	vec3 normal = normalize(f_NormalDepth.xyz);
	
	o_Color = vec3(0.6f, 0.5f, 0.3f);
	o_NormalDepth = vec4(normal, f_NormalDepth.w);
	o_PositionVS = f_PositionVS;
}

