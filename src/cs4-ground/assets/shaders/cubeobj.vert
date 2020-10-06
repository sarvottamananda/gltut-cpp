#version 430 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 texpos;

uniform mat4 mvp;

out vec4 f_texpos;

void main()
{
    gl_Position = mvp * vec4(pos);

    //fcol = vec4(0.5+0.5*pos.x, 0.5+0.5*pos.y, 0.5+0.5*pos.z, 1.0);

    f_texpos = texpos;

}

