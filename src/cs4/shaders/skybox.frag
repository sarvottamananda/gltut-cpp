#version 430 core
out vec4 fragcolor;

in vec4 texcoords;

uniform samplerCube skybox;

void main()
{    
    fragcolor = texture(skybox, texcoords.stp);
}
