#version 330

uniform vec3 Scale;
uniform mat4 Transform;
uniform mat4 ViewProjection;

in vec3 Position;
in vec3 Normal;

out vec3 f_Normal;

void main()
{	
	vec3 scaled = Position * Scale;
	vec4 transformed = ViewProjection * Transform * vec4(scaled, 1.0f);

	gl_Position = transformed;
	f_Normal = (Transform * vec4(Normal, 0.0f)).xyz;
}

