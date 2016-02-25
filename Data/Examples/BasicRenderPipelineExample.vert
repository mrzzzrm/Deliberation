#version 330

uniform mat4 ViewProjection;
uniform mat4 Transform;

in vec3 Position;
in vec3 Normal;

out vec4 f_NormalDepth;

void main()
{
	gl_Position = ViewProjection * Transform * vec4(Position, 1.0f);
	f_NormalDepth = vec4(((Transform * vec4(Normal, 0.0f)).xyz, gl_Position.z / gl_Position.w));
}

