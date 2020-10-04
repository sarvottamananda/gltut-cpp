#version 400 core
layout (location = 0) in vec4 apos;

out vec4 texcoords;

uniform mat4 vp;

void main()
{
    texcoords = apos;
    vec4 vpos = vp * vec4(apos);
    gl_Position = vpos.xyww;
}  
