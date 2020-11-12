#version 430 core

uniform samplerCube skybox_tex;

//const float refr_coef = 0.4;

in vec4 texcoords;

out vec4 fragcolor;

void main()
{    

    // HACK 
    //
    // Since our far point is not at infinity, we hardcode how much we can look below the horizon from our current
    // location. The following has an effect of full transparency at the horizon.
    
    //fragcolor =  texture(skybox_tex, vec3(texcoords.st, -texcoords.p));
    //fragcolor =  (refr_coef + smoothstep(-0.05, -0.005, texcoords.y) * (1 - refr_coef)) * texture(skybox_tex, vec3(tc.st, -tc.p));
    //fragcolor.a = 1.0;

    vec3 tc = vec3(texcoords);

    if (texcoords.y < 0) {
        fragcolor =  texture(skybox_tex, vec3(tc.s, -tc.t, -tc.p));
    } else {
        fragcolor =  texture(skybox_tex, vec3(tc.s, tc.t, -tc.p));
    }


}
