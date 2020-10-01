#version 400 core
out vec4 fragcolor;

in vec4 texcoords;

uniform samplerCubeArray skybox;
uniform float cubemap;

void main()
{    
    fragcolor = texture(skybox, vec4(texcoords.xyz, cubemap), 0);
}
