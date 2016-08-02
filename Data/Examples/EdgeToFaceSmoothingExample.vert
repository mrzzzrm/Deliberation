#version 330

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Transform;

in vec3 Position;
in vec3 Normal;

out vec3 f_Normal;
out vec3 f_VS_Position;

void main()
{
	vec4 vs_position = View * Transform * vec4(Position, 1.0f);
	gl_Position = Projection * vs_position;
	f_VS_Position = vs_position.xyz;
	f_Normal = ((Transform * vec4(Normal, 0.0f)).xyz);
}

