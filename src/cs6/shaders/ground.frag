#version 430 core

uniform sampler2D ground_tex;
uniform samplerCube skybox_tex;
uniform vec3 eye_pos;

in vec4 f_pos;

out vec4 fragcolor;

void main()
{
    vec2 st;
    st = vec2(f_pos.x/f_pos.w, f_pos.y/f_pos.w)/4;

    vec4 gcolor;
    gcolor = 0.2*texture(ground_tex, st);

    vec4 skycolor;
    vec3 gpos;
    vec3 epos;
    gpos.x = f_pos.x/f_pos.w;
    gpos.y = 0;
    gpos.z = f_pos.y/f_pos.w;

    epos = eye_pos;
    epos.z = -epos.z;

    vec3 dir = gpos - epos;

    dir[1] = -dir[1];

    dir = normalize(dir);

    skycolor = 0.8*texture(skybox_tex, vec3(dir.s, dir.t, dir.p));

    fragcolor = gcolor + skycolor;
    fragcolor.a = 0.3;
    //fragcolor = skycolor;


}
