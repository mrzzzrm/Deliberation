#version 330

uniform mat4 View;
uniform mat4 Projection;

in vec3 Position;

out vec3 f_UVW;

void main()
{
    vec4 pos = Projection * View * vec4(Position, 1.0);
    f_UVW = Position;

    // no model, no projection - our coordinates are device coordinates already
    gl_Position = pos.xyww;
}
