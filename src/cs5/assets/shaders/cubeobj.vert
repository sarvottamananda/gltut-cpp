#version 430 core

struct Model_data {
    mat4 mvp;
    mat4 model;
    mat4 model_invxpos;
    ivec4 material;
};

layout (std140) uniform model_block {
    Model_data mdata[64];
};

uniform vec3 eye_pos;
uniform vec3 sun_dir;

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 texpos;

out vec4 f_texpos;
out vec3 f_normal;
out vec3 f_pos;
out flat int s_ind;
out flat int t_ind;

void main()
{
    int inst = gl_InstanceID;

    if (inst < 64){
        gl_Position = mdata[inst].mvp * vec4(pos);
        //gl_Position = mdata[inst].model_invxpos * pos;

        f_texpos = texpos;

        f_normal = vec3(mdata[inst].model_invxpos * normalize(normal));

        f_pos = vec3(mdata[inst].model * pos);

        int mat_ind =  mdata[inst].material[0];
        s_ind = mat_ind / 4;
        t_ind = mat_ind % 4;

    }
}
