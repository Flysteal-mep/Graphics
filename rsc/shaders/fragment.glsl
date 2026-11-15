#version 460 core

in vec2 UV;
out vec4 FragColor;

uniform sampler2D texture_u;

void main()
{
    FragColor = texture(texture_u, UV);
}
