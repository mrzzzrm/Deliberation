#version 330

in vec4 f_NormalDepth;

out vec3 o_Color;
out vec4 o_NormalDepth;

void main()
{
	o_Color = vec3(0.6f, 0.5f, 0.3f) * dot(normalize(f_NormalDepth.xyz), vec3(0.1f, 1.0f, 1.0f));
	o_NormalDepth = f_NormalDepth;
}

