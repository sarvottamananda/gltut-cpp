#version 400 core
layout (location = 0) in vec3 apos;

out vec4 texcoords;

uniform mat4 vp;

void main()
{
    texcoords = vec4(apos, 0.0);
    vec4 vpos = vp * vec4(apos, 1);
    gl_Position = vpos.xyww;
}  
