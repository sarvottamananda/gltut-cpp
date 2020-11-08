// Sarvottamananda (shreesh)
// 2020-09-28
// app_skybox.cpp v0.0 (OpenGL Code Snippets)
//
// Apps derived from App_base

#include "app_skybox.h"

// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "cs3_config.h"
#include "img_stuff.h"
#include "model_cube.h"
#include "shader_stuff.h"
#include "window.h"
#include <cmath>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
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

int nskyboxes = 0;
Model_cube cube;
GLuint vao = 0;
GLuint vbo = 0;
GLuint ebo = 0;
GLuint mvp_id = 0;
GLuint vp_id = 0;
GLuint skybox_loc = 0;
GLuint cubemap_loc = 0;
GLuint skybox_prog = 0;
GLuint cubeobj_prog = 0;
GLuint skybox_txtr = 0;
glm::mat4 mvp = glm::mat4(1.0f);
glm::mat4 vp = glm::mat4(1.0f);

using glm::vec3;

auto pos = vec3(0.0f, -4.0f, 0.0f);  // model location
auto sf = vec3(1.0f, 1.0f, 1.0f);    // model scaling factor
auto angle = (GLfloat)0.0f;          // model rotation angle
auto axis = vec3(1.0f, 1.0f, 1.0f);  // model rotational axis

auto eye_up = vec3(0, 1, 0);      // Up is +ive Y (will be (0,-1,0) to look upside-down)
auto eye_right = vec3(1, 0, 0);   // Right is +ive X
auto eye_front = vec3(0, 0, -1);  // Front is -ive Z
auto eye_dist = start_dist;

auto eye_pos = vec3(0, 0, start_dist);  // Camera at (8,0,0), in World Space
auto eye_lookat = vec3(0, 0, 0);        // Look at the origin

GLfloat cubemap_num = 0.0f;

}  // unnamed namespace

//
// helper functions
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
static void splice_texture_data(Vector<Image> &image);
static void key_callback(Window *win, int key, int scancode, int action, int mods);
static void rotate_up(float theta);
static void rotate_right(float theta);
static void move_back(float delta);
static void calculate_camera();
static void init_camera();

/*
 * App_skybox::render_loop() is the main function defined in this file.
 */

void
App_skybox::render_loop()
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

    glEnable(GL_DEPTH_TEST);                 // Enables depth testing for triangles in the back
    glEnable(GL_BLEND);                      // Enable Blending for transparency
    glEnable(GL_CULL_FACE);                  // Enables culling of away facing triangles
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  // Remove slight gap between cubemap boundaries
    glEnable(GL_MULTISAMPLE);                // Mix the texture if more pixels are transposing
    glDepthFunc(GL_LEQUAL);                  // Cull farther triangles
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Alpha channed decides the color
                                                        // component of the triangle behind
    glFrontFace(GL_CCW);                                // Faces are counter-clockwise
    glCullFace(GL_BACK);                                // Cull back facing faces
    glClearDepth(1.0f);                                 // Depth buffer clear value
    glClearColor(0.0f, 0.051f, 0.1f, 0.0f);             // Color clear value
    glPointSize(1);  // The points drawn will be one pixel wide

    // The render loop
    while (w.render_cond()) {
        w.render_begin();
        do_draw_commands(w);
        w.render_end();  // This also swaps buffer
    }
}
// render_loop()

static void
prepare(const Window &win)
// Prepare various stuff to draw
{
    prepare_models();
    prepare_programs();  // Programs are not stored in vao
    prepare_uniforms();  // Uniforms are not stored in vao
    prepare_textures();
    prepare_buffers();
    prepare_attributes();  // textures and buffers info is stored in vao
}  // end of prepare()

static void
prepare_programs()
// Create glsl programs for the shader
{
    Vector<string> skybox_shaders = {
        string(cs_config::cs_source_dir) + "/shaders/skybox.vert",
        string(cs_config::cs_source_dir) + "/shaders/skybox.frag",
    };
    Vector<string> cubeobj_shaders = {
        string(cs_config::cs_source_dir) + "/shaders/cubeobj.vert",
        string(cs_config::cs_source_dir) + "/shaders/cubeobj.frag",
    };
    skybox_prog = create_program("Skybox", skybox_shaders);
    cubeobj_prog = create_program("cubeobj", cubeobj_shaders);
}

static void
prepare_uniforms()
{
    vp_id = glGetUniformLocation(skybox_prog, "vp");
    skybox_loc = glGetUniformLocation(skybox_prog, "skybox");
    cubemap_loc = glGetUniformLocation(skybox_prog, "cubemap");
    mvp_id = glGetUniformLocation(cubeobj_prog, "mvp");
}

// Fold: static void prepare_matrices(const Window &win) {{{1
static void
prepare_matrices(const Window &win)
// Comput model, view, project matrices for the cube object and the cubemap
{
    // Model

    glm::mat4 model = glm::mat4(1.0f);  // Identity matrix

    model = glm::scale(model, sf);
    model = glm::rotate(model, glm::radians(angle), axis);
    model = glm::translate(model, pos);

    // View

    glm::mat4 view = glm::lookAt(eye_pos, eye_lookat, eye_up);

    // Projection

    GLfloat fovy = win.get_fovy() * 3;
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

static void
prepare_models()
// We have only one model for both cube map and the cube object
{
    cube.print();
}

// static void prepare_textures() {{{1
static void
prepare_textures()
// Create texture buffers
{
    load_texture_data();

    // Set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //// If we want repeating textures
    // glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
}
// 1}}}

// Fold: load_texture_data() {{{1
static void
load_texture_data()
// Upload pixel data on textures.
{
    Vector<string> file = {
        string(cs_config::cs_root_dir) + "/assets/cubeboxes/cubebox-0.png",
        string(cs_config::cs_root_dir) + "/assets/cubeboxes/cubebox-1.png",
        string(cs_config::cs_root_dir) + "/assets/cubeboxes/cubebox-2.png",
        string(cs_config::cs_root_dir) + "/assets/cubeboxes/cubebox-3.png",
        string(cs_config::cs_root_dir) + "/assets/cubeboxes/cubebox-4.png",
        string(cs_config::cs_root_dir) + "/assets/cubeboxes/cubebox-5.png",
        string(cs_config::cs_root_dir) + "/assets/cubeboxes/cubebox-6.png",
        string(cs_config::cs_root_dir) + "/assets/cubeboxes/cubebox-7.png",
        string(cs_config::cs_root_dir) + "/assets/cubeboxes/cubebox-8.png",
        string(cs_config::cs_root_dir) + "/assets/cubeboxes/cubebox-9.png",
    };
    nskyboxes = file.size();  // Number of skyboxes

    Vector<Image> image(nskyboxes);
    for (int i = 0; i < nskyboxes; i++) {
        image[i].read_file(file[i]);
    }

    splice_texture_data(image);
}

static void
splice_texture_data(Vector<Image> &image)
// Our images are cubemap images in a single file
{
    // For a cube map array, all the sizes of the images should be equal. We assume that it is
    // true, and check it later.

    auto w = image[0].get_width();
    auto h = image[0].get_height();
    auto nc = image[0].get_bytes_per_pixel();
    int fw = w / 4;
    int fh = h / 3;

    std::cout << "No. of cubemaps : " << nskyboxes << "\n";
    std::cout << "Cubemaps resolution : " << fw << " x " << fh << "\n";
    std::cout << "No. of channels : " << nc << "\n";

    // We try to read the relevant parts in a loop, so we tell the looping code where the
    // interesting areas are in the images.
    struct Offset {
        int x;
        int y;
    } loc_face[6] = {
        {2, 1}, {0, 1}, {1, 0}, {1, 2}, {1, 1}, {3, 1},
    };

    // Array to hold the result
    GLubyte *texels = new GLubyte[fw * fh * nc];

    // Create the textures in OpenGL state machine
    glGenTextures(1, &skybox_txtr);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, skybox_txtr);

    // We do not need any mipmap levels for cubemaps as the triangles are at a fixed distance.
    int cnt_mip_level = 1;
    glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, cnt_mip_level, GL_RGBA8, fw, fh, 6 * nskyboxes);

    for (int i = 0; i < nskyboxes; i++) {
        if (image[i].get_width() != w || image[i].get_height() != h ||
            image[i].get_bytes_per_pixel() != nc) {
            std::cerr << "Images do not have same size, resize them equally.\n";
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < 6; j++) {
            GLubyte *p = (GLubyte *)image[i].pixels();

            int scanlines = loc_face[j].y * fh;
            int disp = (loc_face[j].x * fw);

            int offset = (scanlines * w + disp) * nc;

            for (int jj = 0; jj < fh; jj++)
                for (int ii = 0; ii < fw; ii++)
                    for (int kk = 0; kk < nc; kk++) {
                        int tidx = (fw * jj + ii) * nc + kk;
                        int pidx = (jj * w + ii) * nc + kk + offset;

                        // std::cerr << tidx << "\n";

                        if (tidx >= fw * fh * nc || tidx < 0) {
                            std::cerr << "Bounds t : " << tidx << "\n";
                            exit(EXIT_FAILURE);
                        }
                        if (pidx >= w * h * nc || pidx < 0) {
                            std::cerr << "Bounds p : " << pidx << "\n";
                            exit(EXIT_FAILURE);
                        }

                        texels[tidx] = p[pidx];
                    }

            // The first 0 refers to the mipmap level (level 0, since there's only 1)
            // The following 2 zeroes refers to the x and y offsets in case you only want to
            // specify a subrectangle. The final 6i+j refers to the layer index offset (we start
            // from index 0 and have 6 faces). Altogether you can specify a 3D box subset of
            // the overall texture, but only one mip level at a time.

            // std ::cerr << "Loading texture (" << j << " of " << i << "\n";

            // for (int zz = 0 ; zz < fw * fh * nc ; zz++) texels[zz] = 127u;

            glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, i * 6 + j, fw, fh, 1, GL_RGBA,
                            GL_UNSIGNED_BYTE, (void *)texels);
        }
    }

    // glGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);

    // Release the array
    delete[] texels;

    // Free the image data
    for (int i = 0; i < nskyboxes; i++) {
        image[i].free_data();
    }
}
// 1}}}

static void
prepare_buffers()
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

static void
prepare_attributes()
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

    glDepthMask(GL_FALSE);
    glUseProgram(skybox_prog);
    glUniformMatrix4fv(vp_id, 1, GL_FALSE, &vp[0][0]);
    glUniform1i(skybox_loc, 0);
    glUniform1f(cubemap_loc, cubemap_num);
    glCullFace(GL_FRONT);
    glDrawElements(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT, (void *)0);

    glDepthMask(GL_TRUE);
    glUseProgram(cubeobj_prog);
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
    glCullFace(GL_BACK);
    glDrawElements(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT, (void *)0);
}

void
App_skybox::key_callback(Key key, int scancode, Key_action action, Key_mods mods)
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
            cubemap_num = 1.0f;
            break;
        case Key::k2:
            cout << "key (2)\n";
            cubemap_num = 2.0f;
            break;
        case Key::k3:
            cout << "key (3)\n";
            cubemap_num = 3.0f;
            break;
        case Key::k4:
            cout << "key (4)\n";
            cubemap_num = 4.0f;
            break;
        case Key::k5:
            cout << "key (5)\n";
            cubemap_num = 5.0f;
            break;
        case Key::k6:
            cout << "key (6)\n";
            cubemap_num = 6.0f;
            break;
        case Key::k7:
            cout << "key (7)\n";
            cubemap_num = 7.0f;
            break;
        case Key::k8:
            cout << "key (8)\n";
            cubemap_num = 8.0f;
            break;
        case Key::k9:
            cout << "key (9)\n";
            cubemap_num = 9.0f;
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
    eye_pos = -eye_dist * eye_front;
}

static void
init_camera()
{
    eye_right = vec3(1.0f, 0.0f, 0.0f);
    eye_up = vec3(0.0f, 1.0f, 0.0f);
    eye_front = vec3(0.0f, 0.0f, -1.0f);

    angle = 0.0f;
    eye_dist = start_dist;
}
