#version 400 core
out vec4 fragcolor;

in vec3 texcoords;

uniform samplerCube skybox;
uniform float cubemap;

void main()
{    
    fragcolor = texture(skybox, texcoords);
}
