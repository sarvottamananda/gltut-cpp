// Sarvottamananda (shreesh)
// 2020-09-28
// app.cpp v0.0 (OpenGL Code Snippets)
//
// Apps derived from App_base

#include "app.hpp"

// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "img_stuff.hpp"
#include "model_cube.hpp"
#include "shader_stuff.hpp"
#include "window.hpp"
#include <cmath>
#include <iostream>
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

namespace {

constexpr GLfloat delta_theta = 0.005f;
constexpr GLfloat delta = 0.1f;
constexpr GLfloat delta_alpha = 0.5f;
constexpr GLfloat start_dist = 20.0f;

Model_cube cube;
GLuint vao = 0;
GLuint vbo = 0;
GLuint ebo = 0;
GLuint mvp_id = 0;
GLuint vp_id = 0;
GLuint skybox_loc = 0;
GLuint skybox_prog = 0;
GLuint cubeobj_prog = 0;
GLuint skybox_txtr = 0;
glm::mat4 mvp = glm::mat4(1.0f);
glm::mat4 vp = glm::mat4(1.0f);

using glm::vec3;

auto pos = vec3(0.0f, -4.0f, 0.0f);  // model location
auto sf = vec3(1.0f, 1.0f, 1.0f);    // model scaling factor
auto angle = (GLfloat)0.0f;	     // model rotation angle
auto axis = vec3(1.0f, 1.0f, 1.0f);  // model rotational axis

auto eye_up = vec3(0, 1, 0);	  // Up is +ive Y (will be (0,-1,0) to look upside-down)
auto eye_right = vec3(1, 0, 0);	  // Right is +ive X
auto eye_front = vec3(0, 0, -1);  // Front is -ive Z
auto eye_dist = start_dist;

auto eye_pos = vec3(0, 0, start_dist);	// Camera at (8,0,0), in World Space
auto eye_lookat = vec3(0, 0, 0);	// Look at the origin

GLfloat cubemap_num = 0.0f;

}  // unnamed namespace

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
static void prepare_buffers();
static void prepare_attributes();
static void load_texture_data();
static void store_texture_data(Vector<Image> &image);
static void key_callback(Window *win, int key, int scancode, int action, int mods);
static void rotate_up(float theta);
static void rotate_right(float theta);
static void move_back(float delta);
static void calculate_camera();
static void init_camera();

/*
 * App::render_loop() is the main function defined in this file.
 */

void App::render_loop()
// Function for rendering, later on we will make a Renderable class for  doing this.
{
    // This makes w's OpenGL context current, just in case if there are multiple windows too.
    w.make_current();

    // Some information
    std::cout << "OpenGL version : " << glGetString(GL_VERSION) << "\n";
    std::cout << "Renderer : " << glGetString(GL_VERSION) << "\n\n";

    std::cout << "Preparing to draw\n";
    prepare(w);

    // Various OpenGL settings

    glEnable(GL_DEPTH_TEST);		     // Enables depth testing for triangles in the back
    glEnable(GL_BLEND);			     // Enable Blending for transparency
    glEnable(GL_CULL_FACE);		     // Enables culling of away facing triangles
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  // Remove slight gap between cubemap boundaries
    glEnable(GL_MULTISAMPLE);		     // Mix the texture if more pixels are transposing
    glDepthFunc(GL_LEQUAL);		     // Cull farther triangles
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Alpha channel decides the final color
    glFrontFace(GL_CCW);				// Faces are counter-clockwise
    glCullFace(GL_BACK);				// Cull back facing faces
    glClearDepth(1.0f);					// Depth buffer clear value 
    glClearColor(0.0f, 0.051f, 0.1f, 0.0f);		// Color clear value
    glPointSize(1);					// Size of screen points

    // The render loop
    while (w.render_cond()) {
	w.render_begin();
	do_draw_commands(w);
	w.render_end();	 // This also swaps buffer
    }
}
// render_loop()

static void prepare(const Window &win)
// Prepare various stuff to draw
{
    prepare_models();
    prepare_programs();	 // Programs are not stored in vao
    prepare_uniforms();	 // Uniforms are not stored in vao
    prepare_textures();
    prepare_buffers();
    prepare_attributes();  // textures and buffers info is stored in vao
}  // end of prepare()

static void prepare_programs()
// Create glsl programs for the shader
{
    Vector<string> skybox_shaders = {
	"assets/shaders/skybox.vert",
	"assets/shaders/skybox.frag",
    };
    Vector<string> cubeobj_shaders = {
	"assets/shaders/cubeobj.vert",
	"assets/shaders/cubeobj.frag",
    };
    skybox_prog = create_program("Skybox", skybox_shaders);
    cubeobj_prog = create_program("cubeobj", cubeobj_shaders);
}

static void prepare_uniforms()
{
    vp_id = glGetUniformLocation(skybox_prog, "vp");
    skybox_loc = glGetUniformLocation(skybox_prog, "skybox");
    mvp_id = glGetUniformLocation(cubeobj_prog, "mvp");
}

// Fold: static void prepare_matrices(const Window &win) {{{1
static void prepare_matrices(const Window &win)
// Comput model, view, project matrices for the cube object and the cubemap
{
    // Model

    glm::mat4 model = glm::mat4(1.0f);	// Identity matrix

    model = glm::scale(model, sf);
    model = glm::rotate(model, glm::radians(angle), axis);
    model = glm::translate(model, pos);

    // View

    glm::mat4 view = glm::lookAt(eye_pos, eye_lookat, eye_up);

    // Projection

    GLfloat fovy = win.get_fovy() * 2;
    GLfloat aspect = win.get_aspect();
    GLfloat near = 0.1f;
    GLfloat far = 100.0f;

    glm::mat4 projection = glm::perspective(fovy, aspect, near, far);

    // mvp for cube object

    mvp = projection * view * model;

    // Remove translation for the camera

    view = glm::mat4(glm::mat3(view));

    // vp  for cubemap

    vp = projection * view;
}
// 1}}}

static void prepare_models()
// We have only one model for both cube map and the cube object
{
    cube.print();
}

// static void prepare_textures() {{{1
static void prepare_textures()
// Create texture buffers
{
    load_texture_data();

    // Set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //// If we want repeating textures
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
}
// 1}}}

// Fold: load_texture_data() {{{1
static void load_texture_data()
// Upload pixel data on textures.
{
    Vector<string> file = {
	"assets/textures/skybox-right.jpg", "assets/textures/skybox-left.jpg",
	"assets/textures/skybox-top.jpg", "assets/textures/skybox-bottom.jpg",
	"assets/textures/skybox-front.jpg", "assets/textures/skybox-back.jpg",
    };
    auto num_images = file.size();  // Number of images

    Vector<Image> image(num_images);
    for (int i = 0; i < num_images; i++) {
	image[i].read_file(file[i]);
    }
    store_texture_data(image);
}

static void store_texture_data(Vector<Image> &image)
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
    glGenTextures(1, &skybox_txtr);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_txtr);

    // We do not need any mipmap levels for cubemaps as the triangles are at a fixed distance.
    int cnt_mip_level = 1;
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, cnt_mip_level, GL_RGB8, w, h);

    for(auto i = 0; i< num_images; i++) {
	// Array holding the texels
	GLubyte *p = (GLubyte *)image[i].pixels();

	if (image[i].get_width() != w || image[i].get_height() != h ||
	    image[i].get_bytes_per_pixel() != nc) {
	    std::cerr << "Images do not have same size, resize them equally.\n";
	    exit(EXIT_FAILURE);
	}

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, w, h,
			    GL_RGB, GL_UNSIGNED_BYTE, (void *)p);
    }
    //glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // Free the image data
    for (int i = 0; i < num_images; i++) {
	image[i].free_data();
    }
}
// 1}}}

static void prepare_buffers()
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, cube.v_num * sizeof(struct Vertex_data),
		 (const GLvoid *)cube.data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.idx_num * sizeof(GLushort),
		 (const void *)cube.idx, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static void prepare_attributes()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Bind all the buffers here, so that the state information is stored in vao

    // std::cout << "Size : " << sizeof(Vertex_data) << "\n";
    // std::cout << "pos : " << offsetof(Vertex_data, pos) << "\n";
    // std::cout << "normal : " << offsetof(Vertex_data, normal) << "\n";
    // std::cout << "txtr : " << offsetof(Vertex_data, txtr) << "\n";

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_data),
			  (GLvoid *)(offsetof(Vertex_data, pos)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_data),
			  (GLvoid *)(offsetof(Vertex_data, normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_data),
			  (GLvoid *)(offsetof(Vertex_data, txtr)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBindVertexArray(0);
    // note that following is not needed, the call to glVertexAttribPointer is registered

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static void do_draw_commands(const Window &win)
{
    //// Since we are drawing a cubemap we do not need to clear the window,
    //// however we stll need to clear the depth buffer
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);

    calculate_camera();
    prepare_matrices(win);

    glDepthMask(GL_FALSE);
    glUseProgram(skybox_prog);
    glUniformMatrix4fv(vp_id, 1, GL_FALSE, &vp[0][0]);
    glUniform1i(skybox_loc, 0);
    glCullFace(GL_FRONT);
    glDrawElements(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT, (void *)0);

    glDepthMask(GL_TRUE);
    glUseProgram(cubeobj_prog);
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
    glCullFace(GL_BACK);
    glDrawElements(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT, (void *)0);
}

void App::key_callback(Key key, int scancode, Key_action action, Key_mods mods)
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

static void rotate_right(float theta)
{
    eye_front = glm::rotate(eye_front, theta, eye_up);
    eye_right = glm::cross(eye_front, eye_up);
}

static void rotate_up(float theta)
{
    eye_up = glm::rotate(eye_up, theta, eye_right);
    eye_front = glm::cross(eye_up, eye_right);
}

static void move_back(float delta) { eye_dist += delta; }

static void calculate_camera() { eye_pos = -eye_dist * eye_front; }

static void init_camera()
{
    eye_right = vec3(1.0f, 0.0f, 0.0f);
    eye_up = vec3(0.0f, 1.0f, 0.0f);
    eye_front = vec3(0.0f, 0.0f, -1.0f);

    angle = 0.0f;
    eye_dist = start_dist;
}
