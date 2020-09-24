#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tc;

out vec4 fcol;

void main()
{
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);

    //if (gl_VertexID % 3 == 0) gl_Position = vec4(0.5, 0.0, 1.0, 1.0);
    //if (gl_VertexID % 3 == 1) gl_Position = vec4(0.0, 0.5, 1.0, 1.0);
    //if (gl_VertexID % 3 == 2) gl_Position = vec4(-0.5, 0.0, 1.0, 1.0);

    fcol = vec4(1.0, 1.0, 0.0, 1.0);
}

