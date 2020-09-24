#version 400 core

in vec4 fcol;
in vec4 tcoord;

out vec4 fragcolor;

uniform samplerCubeArray  skybox;

void main()
{
    //fragcolor = vec4(0.2, 0.1, 0.05, 1.0)+0.4*vec4(fcol);
    //fragcolor = fcol;
    fragcolor = texture(skybox, tcoord, 0.0);
}
