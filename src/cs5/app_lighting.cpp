/**
 * Sarvottamananda (shreesh)
 * Oct 19, 2020
 * app_lighting.cpp v0.0 (OpenGL Code Snippets)
 *
 * Apps derived from App_base
 */

#include "app_lighting.h"

// clang-format: off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// clang-format: on

#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "img_stuff.h"
#include "model_cube.h"
#include "model_ground.h"
#include "shader_stuff.h"
#include "window.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

//
// using declarations
//

template <class T>
using Vector = std::vector<T, std::allocator<T>>;
using std::string;

//
// unnamed namespace declarations
//

// Fold: local variables {{{1
namespace {

constexpr GLfloat delta_theta = 0.005f;
constexpr GLfloat delta = 0.1f;
constexpr GLfloat delta_alpha = 0.5f;
constexpr GLfloat start_dist = 32.0f;
constexpr GLfloat start_alt = 8.0f;

Model_cube cube;
Model_ground ground;

GLuint vao = 0;
GLuint vbo = 0;
GLuint ebo = 0;

GLuint cmvp_id = 0;
GLuint gmvp_id = 0;
GLuint vp_id = 0;

GLintptr cube_off = 0;
GLintptr cube_base = 0;
GLintptr ground_base = 0;
GLintptr ground_off = 0;

GLuint skybox_tex_loc = 0;
GLuint cube_tex_loc = 0;
GLuint ground_tex_loc = 0;

GLuint skybox_prog = 0;
GLuint cubeobj_prog = 0;
GLuint ground_prog = 0;

GLuint skybox_tex = 0;
GLuint ground_tex = 0;
GLuint cube_tex = 0;

glm::mat4 cmvp = glm::mat4(1.0f);
glm::mat4 gmvp = glm::mat4(1.0f);
glm::mat4 vp = glm::mat4(1.0f);

using glm::vec3;

auto pos = vec3(0.0f, 2.0f, 0.0f);   // model location
auto sf = vec3(1.0f, 1.0f, 1.0f);    // model scaling factor
auto angle = (GLfloat)0.0f;	     // model rotation angle
auto axis = vec3(1.0f, 1.0f, 1.0f);  // model rotational axis

auto eye_up = vec3(0, 1, 0);	  // Up is +ive Y (will be (0,-1,0) to look upside-down)
auto eye_right = vec3(1, 0, 0);	  // Right is +ive X
auto eye_front = vec3(0, 0, -1);  // Front is -ive Z
auto eye_dist = start_dist;
auto eye_alt = start_alt;

auto eye_pos = vec3(0, start_alt, start_dist);	// Camera at (8,0,0), in World Space
auto eye_lookat = vec3(0, start_alt, 0);	// Look at the origin

GLfloat cubemap_num = 0.0f;

}  // unnamed namespace

// 1}}}

//
// helper functions: in order of their use
//

static void prepare(const Window &);
static void do_draw_commands(const Window &);
static void prepare(const Window &win);
static void prepare_models();
static void prepare_matrices(const Window &);
static void prepare_programs();
static void prepare_uniforms();
static void prepare_textures();
static void prepare_cubemap_texture();
static void prepare_ground_texture();
static void prepare_cube_texture();
static void prepare_buffers();
static void prepare_attributes();
static void store_cubemap_texture_data(Vector<Image> &image);
static void key_callback(Window *win, int key, int scancode, int action, int mods);
static void rotate_up(float theta);
static void rotate_right(float theta);
static void move_back(float delta);
static void calculate_camera();
static void init_camera();
static void GLAPIENTRY debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
				      GLsizei length, const GLchar *message,
				      const void *userParam);

/*
 * App::render_loop() is the main function defined in this file.
 */

// App::render_loop() {{{1
void
App_lighting::render_loop()
// Function for rendering, later on we will make a Renderable class for  doing this.
{
    // This makes w's OpenGL context current, just in case if there are multiple windows too.
    w.make_current();

    // Various OpenGL settings, can be set only after window is created and made current

    // Some information
    std::cout << "OpenGL version : " << glGetString(GL_VERSION) << "\n";
    std::cout << "Renderer : " << glGetString(GL_VERSION) << "\n\n";

    glEnable(GL_DEPTH_TEST);		     // Enables depth testing for triangles in the back
    glEnable(GL_BLEND);			     // Enable Blending for transparency
    glEnable(GL_CULL_FACE);		     // Enables culling of away facing triangles
    glCullFace(GL_BACK);		     // Cull back facing faces
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  // Remove slight gap between cubemap boundaries
    glEnable(GL_MULTISAMPLE);		     // Mix the texture if more pixels are transposing
    glEnable(GL_DEBUG_OUTPUT);		     // Debug require OpenGL 4.3
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);   // Flush debug messages

    glDebugMessageCallback(debug_callback, nullptr);	// Simple debug callback
    glDepthFunc(GL_LEQUAL);				// Cull farther triangles
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Alpha channel decides the final color
    glFrontFace(GL_CCW);				// Faces are counter-clockwise
    glCullFace(GL_BACK);				// Cull back facing faces
    glClearDepth(1.0f);					// Depth buffer clear value
    glClearColor(0.0f, 0.051f, 0.1f, 0.0f);		// Color clear value
    glPointSize(1);					// Size of screen points

    // Preparing data
    std::cout << "Preparing to draw\n";
    prepare(w);

    // The render loop
    while (w.render_cond()) {
	w.render_begin();
	do_draw_commands(w);
	w.render_end();	 // This also swaps buffer
    }
}  // render_loop()
// 1}}}

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
    prepare_models();
    prepare_programs();	 // Programs are not stored in vao
    prepare_uniforms();	 // Uniforms are not stored in vao
    prepare_textures();
    prepare_buffers();
    prepare_attributes();  // textures and buffers info is stored in vao
}  // end of prepare()

static void
prepare_models()
// We have only one model for both cube map and the cube object
{
    cube.print();
    ground.print();
}

static void
prepare_programs()
// Create glsl programs for the shader
{
    Vector<string> skybox_shaders = {
	"assets/shaders/skybox.vert",
	"assets/shaders/skybox.frag",
    };
    skybox_prog = create_program("Skybox", skybox_shaders);

    Vector<string> ground_shaders = {
	"assets/shaders/ground.vert",
	"assets/shaders/ground.frag",
    };
    ground_prog = create_program("Ground", ground_shaders);

    Vector<string> cubeobj_shaders = {
	"assets/shaders/cubeobj.vert",
	"assets/shaders/cubeobj.frag",
    };
    cubeobj_prog = create_program("Cubeobj", cubeobj_shaders);
}

static void
prepare_uniforms()
{
    vp_id = glGetUniformLocation(skybox_prog, "vp");
    skybox_tex_loc = glGetUniformLocation(skybox_prog, "skybox");

    cmvp_id = glGetUniformLocation(cubeobj_prog, "mvp");
    cube_tex_loc = glGetUniformLocation(cubeobj_prog, "cube_tex");

    gmvp_id = glGetUniformLocation(ground_prog, "mvp");
    ground_tex_loc = glGetUniformLocation(ground_prog, "ground_tex");
}

// Fold: prepare_matrices(const Window &win) {{{1
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

    glm::mat4 view = glm::lookAt(eye_pos, eye_lookat, eye_up);

    // Projection

    GLfloat fovy = win.get_fovy() * 2;
    GLfloat aspect = win.get_aspect();
    GLfloat near = 1 / 1024.0f;
    GLfloat far = 1024.0f;

    glm::mat4 projection = glm::perspective(fovy, aspect, near, far);

    // Model matric for cube

    glm::mat4 cmodel = glm::mat4(1.0f);	 // Identity matrix

    cmodel = glm::scale(cmodel, sf);
    cmodel = glm::rotate(cmodel, glm::radians(angle), axis);
    cmodel = glm::translate(cmodel, pos);

    // mvp for cube object

    cmvp = projection * view * cmodel;

    // Model matric for ground

    glm::mat4 gmodel = glm::mat4(1.0f);	 // Identity matrix

    gmodel = glm::scale(gmodel, vec3(1.0f, 1.0f, 1.0f));
    gmodel = glm::rotate(gmodel, glm::radians(90.0f), vec3(-1.0f, 0.0f, 0.0f));
    gmodel = glm::translate(gmodel, vec3(0.0f, 0.0f, 0.0f));

    // mvp for ground

    gmvp = projection * view * gmodel;

    // Remove translation for the camera

    view = glm::mat4(glm::mat3(view));

    // vp  for cubemap

    vp = projection * view;
}
// 1}}}

// static void prepare_textures() {{{1
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
// 1}}}

// Fold: prepare_cupemap_texture() {{{1
static void
prepare_cubemap_texture()
// Upload pixel data on textures.
{
    Vector<string> file = {
	"assets/textures/skybox-right.jpg", "assets/textures/skybox-left.jpg",
	"assets/textures/skybox-top.jpg",   "assets/textures/skybox-bottom.jpg",
	"assets/textures/skybox-front.jpg", "assets/textures/skybox-back.jpg",
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
// 1}}}

// Fold: prepare_ground_texture() {{{1
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
// 1}}}

static void
prepare_cube_texture()
{
    Image image;

    image.read_file("assets/textures/cc-textures.jpg");

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

// prepare_buffers() {{{1
static void
prepare_buffers()
{
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
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
// 1}}}

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

// do_draw_commands(const Window &win) {{{1
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

    // Draw skybox
    // /*
    glUseProgram(skybox_prog);
    glUniformMatrix4fv(vp_id, 1, GL_FALSE, &vp[0][0]);
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
    glUniformMatrix4fv(gmvp_id, 1, GL_FALSE, &gmvp[0][0]);

    glDisable(GL_CULL_FACE);  // Enables culling of away facing triangles
    glDrawElementsBaseVertex(GL_TRIANGLES, ground.idx_num, GL_UNSIGNED_SHORT,
			     (void *)ground_off, (GLint)ground_base);
    glEnable(GL_CULL_FACE);  // Enables culling of away facing triangles
    // */

    // Draw cube
    // /*
    glUseProgram(cubeobj_prog);
    glUniform1i(cube_tex_loc, 2);
    glUniformMatrix4fv(cmvp_id, 1, GL_FALSE, &cmvp[0][0]);
    glCullFace(GL_BACK);
    // glDrawElements(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT, (void *)0);

    glDrawElementsBaseVertex(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT, (void *)cube_off,
			     (GLint)cube_base);
    // */
}
// 1}}}

// App_lighting::key_callback {{{1
void
App_lighting::key_callback(Key key, int scancode, Key_action action, Key_mods mods)
{
    using std::cout;

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
	    angle += delta_alpha;
	    break;
	case Key::kd:
	    cout << "key (d)\n";
	    angle -= delta_alpha;
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
// 1}}}

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
calculate_camera()
{
    eye_pos = -eye_dist * eye_front + eye_alt * vec3(0.0f, 1.0f, 0.0f);
}
static void
init_camera()
{
    eye_right = vec3(1.0f, 0.0f, 0.0f);
    eye_up = vec3(0.0f, 1.0f, 0.0f);
    eye_front = vec3(0.0f, 0.0f, -1.0f);

    angle = 0.0f;
    eye_dist = start_dist;
    eye_alt = start_alt;
}

void
App_lighting::initialize(Options &os)
{
    w.initialize("OpenGL Snippets : Textured Ground", os.width, os.height, os.fullscreen);
}
