#version 330

uniform mat4 ViewProjectionInverted;

in vec3 Position;

out vec3 f_UVW;

void main()
{
    f_UVW = (ViewProjectionInverted * vec4(Position, 1.0)).xyz;
    //f_UVW.x = -f_UVW.x; // we want to see the cube from the inside

    // no model, no projection - our coordinates are device coordinates already
    gl_Position = vec4(Position, 1.0);
}
