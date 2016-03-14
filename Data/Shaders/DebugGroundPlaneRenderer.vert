#version 330

uniform float Size;
uniform mat4 View;
uniform mat4 Projection;
uniform float FarPlaneZ;

in vec3 Position;

out float f_Depth;
out vec3 f_Normal;
out vec2 f_PositionNormalized;
out vec3 f_PositionVS;

void main()
{	
	vec4 positionVS = View * vec4(Size * Position, 1.0f);
	f_Depth = -positionVS.z / FarPlaneZ;
	f_Normal = (View * vec4(0.0f, 1.0f, 0.0f, 0.0f)).xyz;
	f_PositionVS = positionVS.xyz;
	f_PositionNormalized = (Position.xz + 1.0f) / 2.0f;
	gl_Position = Projection * positionVS;
}

