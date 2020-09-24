#version 330 core

in vec4 fcol;
out vec4 fragcolor;

void main()
{
    fragcolor = vec4(0.2, 0.1, 0.05, 1.0)+0.4*vec4(fcol);
}
