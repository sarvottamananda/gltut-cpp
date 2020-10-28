#version 430 core

out vec4 fragcolor;
in vec4 f_pos;

uniform sampler2D ground_tex;

void main()
{
    vec2 st;

    st = vec2(f_pos.x/f_pos.w, f_pos.y/f_pos.w)/4;
    fragcolor = 0.66*texture(ground_tex, st);
    fragcolor.a = 1.0;
}
