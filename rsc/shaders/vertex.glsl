#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float uScale = 0.2f;

out vec3 Color;

void main()
{
    Color = aColor;
    gl_Position = vec4((aPos) * uScale, 1.0);
}
