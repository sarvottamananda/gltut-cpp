#version 430 core
out vec4 fragcolor;

in vec4 texcoords;

uniform samplerCube skybox_tex;

void main()
{    
    fragcolor = texture(skybox_tex, vec3(texcoords.st, -texcoords.p));
    if (texcoords.y < 0) {
        fragcolor = 0.2 * fragcolor;
        fragcolor.a = 0.8;
    }

}
