#version 330

in vec3 Position;

uniform GlobalBlock
{
    mat4 ViewProjection;
    float Time;
    vec3 Offset;
};

uniform LocalBlock
{
    mat4 Transform;
    int Type;
} local;

uniform vec3 Color;

void main()
{
    float scale = 1.0f;
    
    if (local.Type == 1) 
    {
        scale = abs(cos(Time)) + 0.4f;
    }
    
    gl_Position = (ViewProjection * local.Transform * vec4((scale * Position + Offset), 1.0f));
}

