#version 430 core
out vec4 fragcolor;

in vec4 texcoords;

uniform samplerCube skybox;

void main()
{    
    fragcolor = texture(skybox, vec3(texcoords.st, -texcoords.p));
}
