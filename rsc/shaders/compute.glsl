#version 430 core

layout (local_size_x = 8, local_size_y = 8) in;

layout(rgba32f, binding = 0) uniform image2D Image;

void main()
{
    vec4 value = vec4(1.0, 0.0, 1.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    value.x = float(texelCoord.x) / (gl_NumWorkGroups.x);
    value.y = float(texelCoord.y) / (gl_NumWorkGroups.y);

    imageStore(Image, texelCoord, value);
}
