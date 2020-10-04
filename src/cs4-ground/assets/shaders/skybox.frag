#version 400 core
out vec4 fragcolor;

in vec4 texcoords;

uniform samplerCube skybox;
uniform float cubemap;

void main()
{    
    fragcolor = texture(skybox, texcoords.stp);
}
