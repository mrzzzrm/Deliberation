#version 330

uniform mat4 Projection;

in vec2 Position;

out vec2 f_UV;
out vec3 f_ViewRay;

void main()
{
	gl_Position = vec4(Position, 0.0f, 1.0f);
	
	f_UV = (Position + 1.0f) / 2.0f;
	
	vec4 viewRay = inverse(Projection) * vec4(Position, 1.0f, 1.0f);
	f_ViewRay = viewRay.xyz / viewRay.w;
}

