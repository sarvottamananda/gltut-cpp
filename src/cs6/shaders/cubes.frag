#version 430 core

uniform sampler2D cube_tex;

uniform vec3 eye_pos;
uniform vec3 sun_dir;
uniform vec3 ambient_color;
uniform vec3 sun_color;

const float ambient_coeff = 0.5;
const float diffuse_coeff = 1.0;
const float specular_coeff = 0.75;
const float specular_shine = 4.0;
const float refl_coef = 0.75;

in vec4 f_texpos;
in vec3 f_normal;
in vec3 f_pos;
in flat int s_ind;
in flat int t_ind;

out vec4 frag_color;

vec3 ambient_comp;
vec3 diffuse_comp;
vec3 specular_comp;
vec3 refl_diffuse_comp;
vec3 refl_specular_comp;

void main()
{
    vec2 tex_off = vec2(s_ind * 0.25, t_ind * 0.25);
    vec4 color = texture(cube_tex, tex_off + 0.25 * f_texpos.st);
    vec3 refl_sun_dir = vec3(sun_dir.x, -sun_dir.y, sun_dir.z);

    ambient_comp = ambient_coeff * ambient_color;

    float diff = max(dot(f_normal, sun_dir), 0.0);
    diffuse_comp = diff * sun_color ;

    diff = max(dot(f_normal, refl_sun_dir), 0.0);
    refl_diffuse_comp = refl_coef * diff * sun_color ;

    vec3 view_dir = normalize(eye_pos - f_pos);
    vec3 reflect_dir = normalize(reflect(normalize(-sun_dir), normalize(f_normal)));
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), specular_shine);
    specular_comp = specular_coeff * spec * sun_color;

    reflect_dir = normalize(reflect(normalize(-refl_sun_dir), normalize(f_normal)));
    spec = pow(max(dot(view_dir, reflect_dir), 0.0), specular_shine);
    refl_specular_comp = refl_coef * specular_coeff * spec * sun_color;

    vec3 res = (ambient_comp + diffuse_comp + specular_comp 
            + refl_diffuse_comp + refl_specular_comp) * color.rgb;
    //vec3 res = (specular_comp) ;
    //vec3 res = (ambient_comp) * color.rgb;
    //vec3 res = (diffuse_comp) * color.rgb;

    frag_color = vec4(res, 1.0);
    //frag_color = color;
}
