#version 330

uniform mat4 View;
uniform mat4 ViewProjection;

in vec3 Position;
in vec3 LightPosition;
in vec3 Intensity;
in int  Active;

out vec3 f_LightPosition;
out vec3 f_Intensity;
//out vec2 f_UV;
out float f_Radius;

void main()
{
    if (Active == 0)
    {
        gl_Position = vec4(0.0f);
    }
    else
    {
        float maxIntensity = max(max(Intensity.x, Intensity.y), Intensity.z);
        float radius = sqrt(maxIntensity * 1000.0f);
        f_Radius = radius;

        vec4 position = ViewProjection * vec4(LightPosition + radius * Position, 1.0f);
        gl_Position = position;
        f_LightPosition = (View * vec4(LightPosition, 1.0f)).xyz;
        f_Intensity = Intensity;

//        vec4 uv = position;
//        uv.xy /= position.w;
//        uv.xy = (uv.xy + 1.0f) / 2.0f;
//        f_UV = uv.xy;
    }
}
