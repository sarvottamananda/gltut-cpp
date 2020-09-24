#version 330 core

in vec4 fcol;
out vec4 fragcolor;

void main()
{
    fragcolor = vec4(fcol);
}
