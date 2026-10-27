#version 460 core

layout(location = 0) in vec2 aPos;

out vec3 aColor;

void main()
{
    aColor = vec3(aPos.xy, 0.0);
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
}
