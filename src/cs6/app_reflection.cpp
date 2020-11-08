/**
 * Sarvottamananda (shreesh)
 * 2020-11-08
 * app_reflection.cpp v0.0 (OpenGL Code Snippets)
 *
 * Apps derived from App_base
 */

#include "app_reflection.h"

// clang-format: off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// clang-format: on

#include <chrono>
#include <cmath>
#include <glm/geometric.hpp>
#include <iostream>
#include <random>
#include <vector>

#include "cs6_config.h"
#include "img_stuff.h"
#include "model_cube.h"
#include "model_ground.h"
#include "opengl_stuff.h"
#include "shader_stuff.h"
#include "window.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

//
// using declarations
//

template <class T>

using Vector = std::vector<T, std::allocator<T>>;
using std::cout;
using std::string;

using glm::ivec4;
using glm::mat4;
using glm::vec3;

//
// unnamed namespace declarations
//

namespace {

// camera interactive movement
constexpr GLfloat delta_theta = 0.005f;  // delta angle
constexpr GLfloat delta = 0.1f;          // delta distance
constexpr GLfloat start_dist = 60.0f;    // start position on +z axis looking towards -z
constexpr GLfloat start_alt = 8.0f;      // start at certain altitude to look above

// models
Model_cube cube;      // test model and the skybox
Model_ground ground;  // flat ground

// program objects
GLuint skybox_prog = 0;   // shader for cubemap
GLuint cubes_prog = 0;  // shader for lighted test object
GLuint refls_prog = 0;  // shader for lighted test object
GLuint ground_prog = 0;   // shader for ground

// various array object
GLuint vao = 0;  // one vertex array object for all

// various buffer objects
GLuint vbo = 0;
GLuint ebo = 0;
GLuint model_ubo = 0;

// uniform block(s)
GLuint cmodel_ind = 0;
GLuint model_bindpoint = 0;

// uniforms
GLuint gmvp_loc = 0;
GLuint vp_loc = 0;
GLuint eye_pos_loc = 0;
GLuint sun_dir_loc = 0;
GLuint amb_col_loc = 0;
GLuint sun_col_loc = 0;

GLuint skybox_tex_loc = 0;
GLuint cube_tex_loc = 0;
GLuint ground_tex_loc = 0;

// texture objects
GLuint skybox_tex = 0;
GLuint ground_tex = 0;
GLuint cube_tex = 0;

// buffer objects and elments base and offsets
GLintptr cube_off = 0;
GLintptr cube_base = 0;
GLintptr ground_base = 0;
GLintptr ground_off = 0;

// model global data

constexpr GLsizei num_cubes = 1024;  // number of cubes to draw
constexpr GLsizei num_ub = 64;

auto sf = vec3(1.0f, 1.0f, 1.0f);  // model scaling factor

struct Xform {
    vec3 disp;
    GLfloat yvel;
    vec3 axis;
    GLfloat angle;
    GLfloat avel;
} cube_xform[num_cubes];

struct Model_data {
    mat4 mvp;
    mat4 model;
    mat4 model_invxpos;
    ivec4 material;
};

Model_data cmodel[num_cubes];

mat4 gmvp = mat4(1.0f);
mat4 vp = mat4(1.0f);

// world global data

auto box_sz = vec3(120.0f, 80.0f, 60.0f);

auto start_time = std::chrono::steady_clock::now();

GLfloat cubemap_num = 0.0f;  // use only one hires cubemap for this program, multiple cubemap
                             // would have been better, but since we are storing assets locally,
                             // we do not want to copy assets multiple times unnecessarily.

std::random_device rd;
std::mt19937 gen(rd());

// camera global data

auto eye_up = vec3(0, 1, 0);      // Up is +ive Y (will be (0,-1,0) to look upside-down)
auto eye_right = vec3(1, 0, 0);   // Right is +ive X
auto eye_front = vec3(0, 0, -1);  // Front is -ive Z
auto eye_dist = start_dist;
auto eye_alt = start_alt;

auto eye_pos = vec3(0, start_alt, start_dist);  // Camera at (8,0,0), in World Space
auto eye_lookat = vec3(0, start_alt, 0);        // Look at the origin

// lights global data

auto sun_dir = vec3(0.3f, 0.4f, -0.5f);
auto sun_color = vec3(1.0f, 1.0f, 0.75f);
auto ambient_color = vec3(0.5f, 0.75f, 1.0f);

}  // unnamed namespace

//
// helper functions: in order of their use
//

static void prepare(const Window &);
static void do_draw_commands(const Window &);
static void prepare(const Window &win);
static void prepare_world();
static void prepare_models();
static void prepare_cubes();
static void prepare_matrices(const Window &);
static void prepare_programs();
static void prepare_uniforms();
static void prepare_textures();
static void prepare_cubemap_texture();
static void prepare_ground_texture();
static void prepare_cube_texture();
static void prepare_buffers();
static void prepare_array_buffers();
static void prepare_uniform_buffers();
static void prepare_attributes();
static void store_cubemap_texture_data(Vector<Image> &image);
static void key_callback(Window *win, int key, int scancode, int action, int mods);
static void rotate_up(float theta);
static void rotate_right(float theta);
static void move_back(float delta);
static void init_camera();
static void calculate_camera();
static void modify_buffers();
static void GLAPIENTRY debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                      GLsizei length, const GLchar *message,
                                      const void *userParam);

/*
 * App::render_loop() is the main function defined in this file.
 */

void
App_reflection::render_loop()
// Function for rendering, later on we will make a Renderable class for  doing this.
{
    // This makes w's OpenGL context current, just in case if there are multiple windows too.
    w.make_current();

    // Various OpenGL settings, can be set only after window is created and made current

    // Some information
    std::cout << "OpenGL version : " << glGetString(GL_VERSION) << "\n";
    std::cout << "Renderer : " << glGetString(GL_VERSION) << "\n\n";

    glEnable(GL_DEPTH_TEST);                 // Enables depth testing for triangles in the back
    glEnable(GL_BLEND);                      // Enable Blending for transparency
    glEnable(GL_CULL_FACE);                  // Enables culling of away facing triangles
    glCullFace(GL_BACK);                     // Cull back facing faces
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  // Remove slight gap between cubemap boundaries
    glEnable(GL_MULTISAMPLE);                // Mix the texture if more pixels are transposing
    glEnable(GL_DEBUG_OUTPUT);               // Debug require OpenGL 4.3
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);   // Flush debug messages

    glDebugMessageCallback(debug_callback, nullptr);    // Simple debug callback
    glDepthFunc(GL_LEQUAL);                             // Cull farther triangles
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Alpha channel decides the final color
    glFrontFace(GL_CCW);                                // Faces are counter-clockwise
    glCullFace(GL_BACK);                                // Cull back facing faces
    glClearDepth(1.0f);                                 // Depth buffer clear value
    glClearColor(0.0f, 0.051f, 0.1f, 0.0f);             // Color clear value
    glPointSize(1);                                     // Size of screen points

    // Preparing data
    std::cout << "Preparing to draw\n";
    prepare(w);

    // The render loop
    while (w.render_cond()) {
        w.render_begin();
        do_draw_commands(w);
        w.render_end();  // This also swaps buffer
    }
    check_glerror(__FILE__, __LINE__);
}

static void GLAPIENTRY
debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
               const GLchar *message, const void *userParam)
// Directly copied from OpenGL khronos.org website
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

static void
prepare(const Window &win)
// Prepare various stuff to draw
{
    prepare_programs();  // Programs info is not stored in vao

    // first we prepare data
    prepare_world();
    prepare_models();
    prepare_matrices(win);

    // send the data to opengl and prepare to render
    prepare_uniforms();  // Uniforms info is not stored in vao
    prepare_textures();  // textures and buffers info is not stored in vao
    prepare_buffers();
    prepare_attributes();
}

GLfloat
rng_float()
{
    std::uniform_real_distribution<GLfloat> dis(0.0f, 1.0f);
    return dis(gen);
}

static void
prepare_world()
{
    start_time = std::chrono::steady_clock::now();

    sun_dir = glm::normalize(sun_dir);
}

static void
prepare_models()
// We have only one model for both cube map and the cube object
{
    /*
    cube.print();
    ground.print();
    // */
    prepare_cubes();
}

static void
prepare_cubes()
{
    for (auto i = 0u; i < num_cubes; i++) {
        cube_xform[i].disp = vec3(rng_float() * box_sz.x - box_sz.x / 2.0f,
                                  rng_float() * box_sz.y, rng_float() * -box_sz.z);

        vec3 axis_vec = vec3(rng_float(), rng_float(), rng_float());
        if (axis_vec[0] + axis_vec[1] + axis_vec[2] == 0) {
            axis_vec[0] = 1.0;
        }
        cube_xform[i].axis = axis_vec;

        cube_xform[i].angle = rng_float() * 360;
        cube_xform[i].avel = rng_float() * 30;
        cube_xform[i].yvel = 2 + rng_float() * 4;

        GLint material = 4 * int(cube_xform[i].disp.x * 4 / box_sz.x + 2) +
                         int(-cube_xform[i].disp.z * 4 / box_sz.z);
        cmodel[i].material.x = material;
    }

    /*
    std::cout << "Materials : ";
    for (auto i = 0; i < num_cubes; i++){
        std::cout << cmodel[i].material.x << " ";
        std::cout << cube_xform[i].disp.x << " " << cube_xform[i].disp.z  << " ";
    }
    std::cout << std::endl;
    */
}

static void
prepare_programs()
// Create glsl programs for the shader
{
    Vector<string> skybox_shaders = {
        string(cs_config::cs_source_dir) + "/shaders/skybox.vert",
        string(cs_config::cs_source_dir) + "/shaders/skybox.frag",
    };
    skybox_prog = create_program("Skybox", skybox_shaders);

    Vector<string> ground_shaders = {
        string(cs_config::cs_source_dir) + "/shaders/ground.vert",
        string(cs_config::cs_source_dir) + "/shaders/ground.frag",
    };
    ground_prog = create_program("Ground", ground_shaders);

    Vector<string> cubes_shaders = {
        string(cs_config::cs_source_dir) + "/shaders/cubes.vert",
        string(cs_config::cs_source_dir) + "/shaders/cubes.frag",
    };
    cubes_prog = create_program("Cubes", cubes_shaders);

    Vector<string> refls_shaders = {
        string(cs_config::cs_source_dir) + "/shaders/cubes_refl.vert",
        string(cs_config::cs_source_dir) + "/shaders/cubes_refl.frag",
    };
    refls_prog = create_program("Reflections", cubes_shaders);

    // We need 64 Model_data
    GLint info = 0;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &info);
    std::cout << "GL_MAX_UNIFORM_BLOCK_SIZE: " << info << "\n";
    assert(info >= (GLint)(64 * sizeof(Model_data)));

    glGetActiveUniformBlockiv(cubes_prog, 0, GL_UNIFORM_BLOCK_DATA_SIZE, &info);
    std::cout << "GL_UNIFORM_BLOCK_DATA_SIZE (cubes:0) : " << info << "\n";

    glGetActiveUniformBlockiv(refls_prog, 0, GL_UNIFORM_BLOCK_DATA_SIZE, &info);
    std::cout << "GL_UNIFORM_BLOCK_DATA_SIZE (refls:0) : " << info << "\n";

    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &info);
    std::cout << "GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT : " << info << "\n";
}

static void
prepare_uniforms()
{
    // skybox
    skybox_tex_loc = glGetUniformLocation(skybox_prog, "skybox");
    vp_loc = glGetUniformLocation(skybox_prog, "vp");

    // cubes
    cmodel_ind = glGetUniformBlockIndex(cubes_prog, "model_block");
    cube_tex_loc = glGetUniformLocation(cubes_prog, "cube_tex");

    eye_pos_loc = glGetUniformLocation(cubes_prog, "eye_pos");
    sun_dir_loc = glGetUniformLocation(cubes_prog, "sun_dir");
    amb_col_loc = glGetUniformLocation(cubes_prog, "ambient_color");
    sun_col_loc = glGetUniformLocation(cubes_prog, "sun_color");

    glUniformBlockBinding(cubes_prog, cmodel_ind, model_bindpoint);

    // ground
    ground_tex_loc = glGetUniformLocation(ground_prog, "ground_tex");
    gmvp_loc = glGetUniformLocation(ground_prog, "mvp");
}

static void
prepare_matrices(const Window &win)
// Comput model, view, project matrices for the cube object and the cubemap
{
    // View

    /*
    std::cout << eye_pos[0] << " " << eye_pos[1] << " " << eye_pos[2] << "\n";
    std::cout << eye_lookat[0] << " " << eye_lookat[1] << " " << eye_lookat[2] << "\n";
    std::cout << eye_up[0] << " " << eye_up[1] << " " << eye_up[2] << "\n";
    // */

    mat4 view = glm::lookAt(eye_pos, eye_lookat, eye_up);

    // Projection

    GLfloat fovy = win.get_fovy() * 2;
    GLfloat aspect = win.get_aspect();
    GLfloat near = 1 / 1024.0f;
    GLfloat far = 1024.0f;

    mat4 projection = glm::perspective(fovy, aspect, near, far);

    // Model matrix for cube

    auto cur_time = std::chrono::steady_clock::now();
    std ::chrono::duration<float> cur_dur = cur_time - start_time;

    for (auto i = 0u; i < num_cubes; i++) {
        mat4 model = mat4(1.0f);  // Identity matrix

        vec3 pos = cube_xform[i].disp;
        GLfloat angle = cube_xform[i].angle;

        pos[1] -= ((GLfloat)cur_dur.count() * cube_xform[i].yvel);
        angle += ((GLfloat)cur_dur.count() * cube_xform[i].avel);

        // when the cube gets completely below the surface move it back
        if (pos[1] < -2.0f) cube_xform[i].disp[1] += box_sz.y;

        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(angle), cube_xform[i].axis);
        model = glm::scale(model, sf);

        // mvp for cube object

        cmodel[i].mvp = projection * view * model;
        cmodel[i].model = model;
        // remove translation for normal transformation matrix
        mat4 nmodel = mat4(1.0f);
        nmodel = glm::rotate(nmodel, glm::radians(angle), cube_xform[i].axis);
        nmodel = glm::scale(nmodel, sf);
        cmodel[i].model_invxpos = nmodel;
    }

    // Model matrix for ground

    mat4 gmodel = mat4(1.0f);  // Identity matrix

    gmodel = glm::scale(gmodel, vec3(1.0f, 1.0f, 1.0f));
    gmodel = glm::rotate(gmodel, glm::radians(90.0f), vec3(-1.0f, 0.0f, 0.0f));
    gmodel = glm::translate(gmodel, vec3(0.0f, 0.0f, 0.0f));

    // mvp for ground

    gmvp = projection * view * gmodel;

    // Remove translation for the camera

    view = mat4(glm::mat3(view));

    // vp  for cubemap

    vp = projection * view;
}

static void
prepare_textures()
// Create texture buffers
{
    glActiveTexture(GL_TEXTURE0);
    prepare_cubemap_texture();

    glActiveTexture(GL_TEXTURE1);
    prepare_ground_texture();

    glActiveTexture(GL_TEXTURE2);
    prepare_cube_texture();
}

static void
prepare_cubemap_texture()
// Upload pixel data on textures.
{
    Vector<string> file = {
        string(cs_config::cs_root_dir) + "/assets/skybox/skybox-right.jpg",
        string(cs_config::cs_root_dir) + "/assets/skybox/skybox-left.jpg",
        string(cs_config::cs_root_dir) + "/assets/skybox/skybox-top.jpg",
        string(cs_config::cs_root_dir) + "/assets/skybox/skybox-bottom.jpg",
        string(cs_config::cs_root_dir) + "/assets/skybox/skybox-front.jpg",
        string(cs_config::cs_root_dir) + "/assets/skybox/skybox-back.jpg",
    };
    auto num_images = file.size();  // Number of images

    Vector<Image> image(num_images);
    for (auto i = 0u; i < num_images; i++) {
        image[i].read_file(file[i]);
    }
    store_cubemap_texture_data(image);
}

static void
store_cubemap_texture_data(Vector<Image> &image)
// Our images are cubemap images in a single file
{
    // For a cube map array, all the sizes of the images should be equal. We assume that it is
    // true, and check it later.

    auto num_images = image.size();  // Number of images
    auto w = image[0].get_width();
    auto h = image[0].get_height();
    auto nc = image[0].get_bytes_per_pixel();

    assert(nc == 3);
    assert(num_images == 6);

    std::cout << "No. of images : " << num_images << "\n";
    std::cout << "Cubemap resolution : " << w << " x " << h << "\n";
    std::cout << "No. of channels : " << nc << "\n";

    // Create the textures in OpenGL state machine
    glGenTextures(1, &skybox_tex);
    // Set reasonable texture parameters
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // We do not need any mipmap levels for cubemaps as the triangles are at a fixed distance.
    int cnt_mip_level = 1;
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, cnt_mip_level, GL_RGB8, w, h);

    for (auto i = 0u; i < num_images; i++) {
        // Array holding the texels
        GLubyte *p = (GLubyte *)image[i].pixels();

        if (image[i].get_width() != w || image[i].get_height() != h ||
            image[i].get_bytes_per_pixel() != nc) {
            throw std::runtime_error("Images do not have same size, resize them equally");
        }

        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, w, h, GL_RGB,
                        GL_UNSIGNED_BYTE, (void *)p);
    }
    // glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // Free the image data
    for (auto i = 0u; i < num_images; i++) {
        image[i].free_data();
    }
}

static void
prepare_ground_texture()
{
    constexpr GLsizei tex_sz = 64;

    GLubyte image[tex_sz][tex_sz][4];  // RGBA storage

    for (auto i = 0u; i < tex_sz; i++) {
        for (auto j = 0u; j < tex_sz; j++) {
            // clang-format off
	    GLubyte color[4][3] = {
		{ 0, 0, 0, },
		{ 92, 80, 80, },
		{ 208, 192, 192, },
		{ 255, 255, 255, },
	    };
            // clang-format on

            GLubyte m = 0x8;
            GLubyte c = (((i & m * 0x1) == 0) ^ ((j & m * 0x1) == 0));
            GLubyte d = (((i & m * 0x4) == 0) ^ ((j & m * 0x4) == 0));

            auto ind = 2 * d + c;

            image[i][j][0] = (GLubyte)color[ind][0];
            image[i][j][1] = (GLubyte)color[ind][1];
            image[i][j][2] = (GLubyte)color[ind][2];
            image[i][j][3] = (GLubyte)255;
        }
    }

    glGenTextures(1, &ground_tex);
    glBindTexture(GL_TEXTURE_2D, ground_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // we want repeating textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    int cnt_mip_level = 7;
    glTexStorage2D(GL_TEXTURE_2D, cnt_mip_level, GL_RGB8, tex_sz, tex_sz);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex_sz, tex_sz, GL_RGBA, GL_UNSIGNED_BYTE,
                    (void *)image);
    glGenerateMipmap(GL_TEXTURE_2D);
}

static void
prepare_cube_texture()
{
    Image image;

    image.read_file(
        (string(cs_config::cs_root_dir) + "/assets/textures/cc-textures.jpg").c_str());

    auto w = image.get_width();
    auto h = image.get_height();
    auto nc = image.get_bytes_per_pixel();

    assert(nc == 3);

    // glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &cube_tex);
    glBindTexture(GL_TEXTURE_2D, cube_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int cnt_mip_level = 8;
    glTexStorage2D(GL_TEXTURE_2D, cnt_mip_level, GL_RGB8, w, h);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE,
                    (void *)image.pixels());

    image.free_data();
}

static void
prepare_buffers()
{
    GLuint bo[3];

    glGenBuffers(3, bo);
    vbo = bo[0];
    ebo = bo[1];
    model_ubo = bo[2];
    prepare_array_buffers();
    prepare_uniform_buffers();
}

static void
prepare_array_buffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    auto sum_v_num = cube.v_num + ground.v_num;
    auto sum_idx_num = cube.idx_num + ground.idx_num;

    glBufferStorage(GL_ARRAY_BUFFER, sum_v_num * sizeof(struct Vertex_data), (GLvoid *)nullptr,
                    GL_DYNAMIC_STORAGE_BIT);
    glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sum_idx_num * sizeof(GLushort), (GLvoid *)nullptr,
                    GL_DYNAMIC_STORAGE_BIT);

    GLint base = 0;

    GLsizeiptr vbo_off = 0;
    GLsizeiptr ebo_off = 0;

    GLsizeiptr vbo_sz = 0;
    GLsizeiptr ebo_sz = 0;

    // Add cube data

    cube_base = base;
    cube_off = ebo_off;

    vbo_sz = cube.v_num * sizeof(struct Vertex_data);
    ebo_sz = cube.idx_num * sizeof(GLushort);

    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)vbo_off, vbo_sz, (const GLvoid *)cube.data);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (GLintptr)ebo_off, ebo_sz, (const void *)cube.idx);

    vbo_off += vbo_sz;
    ebo_off += ebo_sz;
    base += cube.v_num;

    // Add ground data

    ground_base = base;
    ground_off = ebo_off;

    vbo_sz = ground.v_num * sizeof(struct Vertex_data);
    ebo_sz = ground.idx_num * sizeof(GLushort);

    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)vbo_off, vbo_sz, (const GLvoid *)ground.data);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (GLintptr)ebo_off, ebo_sz,
                    (const void *)ground.idx);

    vbo_off += vbo_sz;
    ebo_off += ebo_sz;
    base += ground.v_num;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static void
prepare_uniform_buffers()
{
    // model matrices uniform block
    glBindBuffer(GL_UNIFORM_BUFFER, model_ubo);
    glBufferStorage(GL_UNIFORM_BUFFER, num_cubes * sizeof(Model_data), (GLvoid *)nullptr,
                    GL_DYNAMIC_STORAGE_BIT);
    glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr)0,
                    (GLsizeiptr)(num_cubes * sizeof(Model_data)), (const GLvoid *)(&cmodel[0]));

    /*
    std::cout << "Materials : ";
    std::cout << sizeof(ivec4) << ":";
    for (auto i = 0; i < num_cubes; i++){
        std::cout << cmaterial[i].x << " ";
    }
    std::cout << std::endl;
    */

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

static void
prepare_attributes()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Bind all the buffers here, so that the state information is stored in vao

    // std::cout << "Size : " << sizeof(Vertex_data) << "\n";
    // std::cout << "pos : " << offsetof(Vertex_data, pos) << "\n";
    // std::cout << "normal : " << offsetof(Vertex_data, normal) << "\n";
    // std::cout << "tx : " << offsetof(Vertex_data, tx) << "\n";

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_data),
                          (GLvoid *)(offsetof(Vertex_data, pos)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_data),
                          (GLvoid *)(offsetof(Vertex_data, normal)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_data),
                          (GLvoid *)(offsetof(Vertex_data, tx)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBindVertexArray(0);
    // note that following is not needed, the call to glVertexAttribPointer is registered

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static void
do_draw_commands(const Window &win)
{
    //// Since we are drawing a cubemap we do not need to clear the window,
    //// however we stll need to clear the depth buffer
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);

    calculate_camera();
    prepare_matrices(win);
    modify_buffers();

    // Draw skybox
    // /*
    glUseProgram(skybox_prog);
    glUniformMatrix4fv(vp_loc, 1, GL_FALSE, &vp[0][0]);
    glUniform1i(skybox_tex_loc, 0);
    glCullFace(GL_FRONT);

    glDepthMask(GL_FALSE);
    glDrawElementsBaseVertex(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT, (void *)cube_off,
                             (GLint)cube_base);
    glDepthMask(GL_TRUE);
    // */

    // Draw ground
    // /*
    glUseProgram(ground_prog);
    glUniform1i(ground_tex_loc, 1);
    glUniformMatrix4fv(gmvp_loc, 1, GL_FALSE, &gmvp[0][0]);

    glDisable(GL_CULL_FACE);  // Enables culling of away facing triangles
    glDrawElementsBaseVertex(GL_TRIANGLES, ground.idx_num, GL_UNSIGNED_SHORT,
                             (void *)ground_off, (GLint)ground_base);
    glEnable(GL_CULL_FACE);  // Enables culling of away facing triangles
    // */

    // Draw cube
    // /*
    glUseProgram(cubes_prog);
    glUniform1i(cube_tex_loc, 2);
    glUniform3fv(eye_pos_loc, 1, glm::value_ptr(eye_pos));
    glUniform3fv(sun_dir_loc, 1, glm::value_ptr(sun_dir));
    glUniform3fv(amb_col_loc, 1, glm::value_ptr(ambient_color));
    glUniform3fv(sun_col_loc, 1, glm::value_ptr(sun_color));
    glCullFace(GL_BACK);
    // glDrawElements(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT, (void *)0);

    /*
    glBindBufferBase(GL_UNIFORM_BUFFER, ub_bindpoint, mvp_ubo);
    glDrawElementsInstancedBaseVertex(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT,
                                      (void *)cube_off, num_ub, (GLint)cube_base);
    */

    for (auto i = 0; i < num_cubes / num_ub; i++) {
        glBindBufferRange(GL_UNIFORM_BUFFER, model_bindpoint, model_ubo,
                          i * num_ub * sizeof(Model_data), num_ub * sizeof(Model_data));
        glDrawElementsInstancedBaseVertex(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT,
                                          (void *)cube_off, num_ub, (GLint)cube_base);
    }
    // *8
}

void
App_reflection::key_callback(Key key, int scancode, Key_action action, Key_mods mods)
{
    if (action == Key_action::release) return;

    switch (key) {
        case Key::left:
            cout << "key (left)\n";
            rotate_right(-delta_theta);
            break;
        case Key::right:
            cout << "key (right)\n";
            rotate_right(delta_theta);
            break;
        case Key::up:
            cout << "key (up)\n";
            rotate_up(delta_theta);
            break;
        case Key::down:
            cout << "key (down)\n";
            rotate_up(-delta_theta);
            break;

        case Key::kw:
            cout << "key (w)\n";
            move_back(-delta);
            break;
        case Key::ks:
            cout << "key (s)\n";
            move_back(delta);
            break;
        case Key::ka:
            cout << "key (a)\n";
            // angle += delta_alpha;
            break;
        case Key::kd:
            cout << "key (d)\n";
            // angle -= delta_alpha;
            break;

        case Key::k0:
            cout << "key (0)\n";
            cubemap_num = 0.0f;
            break;
        case Key::k1:
            cout << "key (1)\n";
            cubemap_num = 0.0f;
            break;
        case Key::k2:
            cout << "key (2)\n";
            cubemap_num = 0.0f;
            break;
        case Key::k3:
            cout << "key (3)\n";
            cubemap_num = 0.0f;
            break;
        case Key::k4:
            cout << "key (4)\n";
            cubemap_num = 0.0f;
            break;
        case Key::k5:
            cout << "key (5)\n";
            cubemap_num = 0.0f;
            break;
        case Key::k6:
            cout << "key (6)\n";
            cubemap_num = 0.0f;
            break;
        case Key::k7:
            cout << "key (7)\n";
            cubemap_num = 0.0f;
            break;
        case Key::k8:
            cout << "key (8)\n";
            cubemap_num = 0.0f;
            break;
        case Key::k9:
            cout << "key (9)\n";
            cubemap_num = 0.0f;
            break;

        case Key::space:
            cout << "key (space)\n";
            init_camera();
            prepare_cubes();
            break;
        case Key::esc:
            cout << "key (esc)\n";
            break;

        case Key::left_shift:
        case Key::right_shift:
            cout << "key (shift)\n";
            break;
        case Key::left_ctrl:
        case Key::right_ctrl:
            cout << "key (ctrl)\n";
            break;
        case Key::left_alt:
        case Key::right_alt:
            cout << "key (alt)\n";
            break;
        case Key::left_super:
        case Key::right_super:
            cout << "key (super)\n";
            break;
        default:
            cout << "key (unknown)\n";
            break;
    }
    return;
}

static void
rotate_right(float theta)
{
    eye_front = glm::rotate(eye_front, theta, eye_up);
    eye_right = glm::cross(eye_front, eye_up);
}

static void
rotate_up(float theta)
{
    eye_up = glm::rotate(eye_up, theta, eye_right);
    eye_front = glm::cross(eye_up, eye_right);
}

static void
move_back(float delta)
{
    eye_dist += delta;
}

static void
init_camera()
{
    eye_right = vec3(1.0f, 0.0f, 0.0f);
    eye_up = vec3(0.0f, 1.0f, 0.0f);
    eye_front = vec3(0.0f, 0.0f, -1.0f);

    eye_dist = start_dist;
    eye_alt = start_alt;

    // angle = 0.0f;
}

static void
calculate_camera()
{
    eye_pos = -eye_dist * eye_front + eye_alt * vec3(0.0f, 1.0f, 0.0f);
}

static void
modify_buffers()
{
    glBindBuffer(GL_UNIFORM_BUFFER, model_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr)0,
                    (GLsizeiptr)(num_cubes * sizeof(Model_data)), (const GLvoid *)(&cmodel[0]));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void
App_reflection::initialize(Options &os)
{
    w.initialize("OpenGL Snippets : Lighted Cubes", os.width, os.height, os.fullscreen);
}
