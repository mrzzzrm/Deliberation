#version 330

uniform mat4 Projection;

in vec2 Position;
in vec2 UV;
in vec4 Color;

out vec2 f_UV;
out vec4 f_Color;

void main()
{
    f_UV = UV;
    f_Color = Color;
    gl_Position = Projection * vec4(Position.xy, 0, 1);
}
