#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tc;

out vec4 fcol;

void main()
{
    gl_Position = vec4(pos.x/2, pos.y/2, 0.0, 1.0);
    fcol = vec4(0.5+0.5*pos.x, 0.5+0.5*pos.y, 0.5+0.5*pos.z, 1.0);
}

