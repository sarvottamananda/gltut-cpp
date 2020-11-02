#version 430 core

in vec4 f_texpos;
out vec4 fragcolor;

uniform sampler2D cube_tex;


void main()
{
    vec2 tex_off = vec2(0.5,0.75);

    fragcolor = vec4(0.30,0.22,0.2,0.4) + 0.6*texture(cube_tex, tex_off + 0.25*f_texpos.st);
}
