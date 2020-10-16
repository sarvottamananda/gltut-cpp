#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tc;

uniform mat4 mvp;

out vec4 fcol;

void main()
{
    gl_Position = mvp * vec4(pos, 1.0);
    fcol = vec4(0.5+0.5*pos.x, 0.5+0.5*pos.y, 0.5+0.5*pos.z, 1.0);
}

