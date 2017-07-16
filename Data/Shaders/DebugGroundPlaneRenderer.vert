#version 330

uniform float Radius;
uniform float Size;
uniform mat4 View;
uniform mat4 Projection;
//uniform float FarPlaneZ;

in vec3 Position;

//out float f_Depth;
out vec3 f_Normal;
out vec2 f_PositionLocal;
out vec2 f_PositionNormalized;
out vec3 f_PositionVS;

void main()
{	
	vec4 positionVS = View * vec4(Size * Position, 1.0f);
	//f_Depth = -positionVS.z / FarPlaneZ;
	f_Normal = (View * vec4(0.0f, 1.0f, 0.0f, 0.0f)).xyz;
	f_PositionVS = positionVS.xyz;
	f_PositionLocal = Position.xz * Size;
	f_PositionNormalized = Position.xz + 0.5f;
	gl_Position = Projection * positionVS;
}

