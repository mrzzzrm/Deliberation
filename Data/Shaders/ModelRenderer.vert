#version 330

uniform View
{
    mat4 View;
    mat4 Projection;
    mat4 ViewProjection;
};

in vec3 Position;
in vec3 Normal;
in vec3 Color;
in mat4 Transform;

out vec3 f_Position;
out vec3 f_Color;
out vec3 f_Normal;

void main()
{
    vec4 positionVS = View * Transform * vec4(Position, 1.0f);
    gl_Position = Projection * positionVS;
    f_Position = positionVS.xyz;
    f_Color = Color;
    f_Normal = Normal;
}