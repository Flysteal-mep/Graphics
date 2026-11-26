#version 460 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D Image;

uniform float uTime = 0.0;

// Tons of thx to @kishimisu his video
// https://youtu.be/khblXafu7iA

// https://iquilezles.org/articles/distfunctions/
float SphereSDF(vec3 point, float r)
{
    return length(point) - r;
}
float BoxSDF(vec3 point, vec3 b)
{
    vec3 q = abs(point) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

vec3 RotationSDF_3D(vec3 point, vec3 axis, float angle)
{
    return mix(dot(axis, point) * axis, point, cos(angle) + cross(axis, point) * sin(angle));
}
mat2 RotationSDF_2D(float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    return mat2(c, -s, s, c);
}

float Map(vec3 point)
{
    float map = 0;

    // normal one
    vec3 spherePos_0 = vec3(sin(uTime), 0.0, 0.0);
    float sphere_0 = SphereSDF(point - spherePos_0, 0.5);
    map = min(sphere_0, 1.0);

    // Scaled shpere
    vec3 spherePos_1 = vec3(cos(uTime), 1.0, 0.0);
    float sphere_1 = SphereSDF((point - spherePos_1) * 4, 0.5) / 4;
    map = min(sphere_1, map);

    // Rotates a box on z axis using
    vec3 boxPos_0 = vec3(0, -1.0, 0.0 - uTime * 0.3);
    vec3 localPos = fract(point - boxPos_0) - 0.5;

    localPos.xy = RotationSDF_2D(uTime) * localPos.xy;
    float box_0 = BoxSDF(localPos, vec3(0.1));
    map = min(box_0, map);


    float plane_0 = point.y + 1.5;
    map = min(plane_0, map);

    return map;
}


void main()
{
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(Image);

    if (any(greaterThanEqual(pixel, size)))
    {
        return;
    }


    vec2 uv = (vec2(pixel) + 0.5) / vec2(size) * 2.0 - 1.0;

    // "camera"
    vec3 rayOrigin = vec3(0.0, 0.0, -3.0);

    float pov = 1.2;

    // Each "camera" pixel path
    vec3 rayDirection = normalize(vec3(uv, 1.0));
    // Total distance that has been traveled by a ray
    float distance = 0;

    // Pixel color
    vec3 color = vec3(0.0);


    for (int i = 0; i < 90; i++)
    {
        // End point of each ray
        vec3 point = rayOrigin + rayDirection * distance;


        // Current distance in the scene
        float scene = Map(point);



        // exit if to small
        if (scene < 0.001)
        {
            color = vec3(1.0, 0.0, 0.4);
            break;
        }

        // March the ray "steps"
        distance += scene;

        if (distance > 100.0)
        {
            break;
        }
    }

    color = vec3(distance * 0.02);


    imageStore(Image, pixel, vec4(color, 1.0));
}
