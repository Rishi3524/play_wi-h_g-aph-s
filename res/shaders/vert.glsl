#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;

out vec4 color;

uniform mat3 xyzMultipliers;

void main()
{
    gl_Position = vec4(xyzMultipliers * vPosition, 1.0);

    color = vec4( vColor, 1.0 );
}
