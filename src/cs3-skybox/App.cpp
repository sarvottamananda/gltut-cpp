/*
    Sarvottamananda (shreesh)
    2020-09-21
    App.cpp v0.0 (OpenGL Code Snippets)

    Apps derived from App_base
*/

#include "App.h"

#include "img_stuff.h"
#include "Model_cube.h"
#include "shader_stuff.h"

#include "iostream"
// clang-format off
#include <GLFW/glfw3.h>
// clang-format on
#include <glm/gtc/matrix_transform.hpp>

using std::string;
template <class T>
using Vector = std::vector<T, std::allocator<T>>;

namespace {

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

}  // namespace

static void prepare(const App_window&);
static void do_draw_commands(const App_window&);

static void test_drawing();

void App::render_loop()
// Function for rendering, later on we will make a Renderable class for  doing this.
{
    // This makes w's OpenGL context current
    w.make_current();
    std::cout << "OpenGL version : " << glGetString(GL_VERSION) << "\n";
    std::cout << "Renderer : " << glGetString(GL_VERSION) << "\n\n";

    std::cout << "Preparing to draw\n";
    prepare(w);

    // Various OpenGL settings

    glClearColor(0.0f, 0.051f, 0.1f, 0.0f);  // Blue Background

    glEnable(GL_DEPTH_TEST);  // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);   // The Type Of Depth Test To Do
    glClearDepth(1.0f);	      // Depth Buffer Setup

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_MULTISAMPLE);

    glPointSize(1);

    // test_drawing();
    
    // The render loop
    while (w.render_cond()) {
	w.render_begin();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	do_draw_commands(w);

	w.render_end();
    }
}
// render_loop()

static void prepare_programs();
static void prepare_textures();
static void prepare_models(const App_window&);
static void prepare_buffers();
static void prepare_attributes();
static void prepare_uniforms();

static void prepare(const App_window& win)
// Prepare various stuff to draw
{
    prepare_programs();
    prepare_models(win);
    prepare_buffers();
    prepare_textures();
    prepare_attributes();
    // Uniforms do not use attributes
    prepare_uniforms();
}  // end of prepare()

static void prepare_programs()
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

static void load_texture_data();

// static void prepare_textures() {{{1
static void prepare_textures()
{
    load_texture_data();

    // Always set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
}
// 1}}}

static void splice_texture_data(Vector<Image> &image);

// Fold: load_texture_data() {{{1
static void load_texture_data()
// Upload pixel data on textures.
{
    Vector<string> file = {
	"assets/textures/cubebox-0.png", "assets/textures/cubebox-1.png",
	"assets/textures/cubebox-2.png", "assets/textures/cubebox-3.png",
	"assets/textures/cubebox-4.png", "assets/textures/cubebox-5.png",
	"assets/textures/cubebox-6.png", "assets/textures/cubebox-7.png",
	"assets/textures/cubebox-8.png", "assets/textures/cubebox-9.png",
    };
    //Vector<string> file = {
	//"assets/textures/cubebox-0.png"
    //};

    nskyboxes = file.size();  // Number of skyboxes

    Vector<Image> image(nskyboxes);

    std::cout << "Reading textures...\n";
    for (int i = 0; i < nskyboxes; i++) {
	image[i].read_file(file[i]);
    }
    std::cout << "\n";

    splice_texture_data(image);
}

static void splice_texture_data(Vector<Image> &image)
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
    ;
    std::cout << "No. of channels : " << nc << "\n";

    struct Offset {
	int x;
	int y;
    };

    Offset loc_face[6] = {
	Offset{2, 1}, Offset{0, 1}, Offset{1, 0}, Offset{1, 2}, Offset{1, 1}, Offset{3, 1},
    };

    GLubyte *texels = new GLubyte[fw * fh * nc];

    // Allocate the storage.
    //
    glGenTextures(1, &skybox_txtr);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, skybox_txtr);

    int cnt_mip_level = 2;
    glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, cnt_mip_level, GL_RGBA8, fw, fh, 6*nskyboxes);

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

	    //for (int zz = 0 ; zz < fw * fh * nc ; zz++) texels[zz] = 127u;

	    glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, i * 6 + j, fw, fh, 1, GL_RGBA,
			   GL_UNSIGNED_BYTE, (void *)texels);
	}
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);

    delete[] texels;

    for (int i = 0; i < nskyboxes; i++) {
	image[i].free_data();
    }
}

// 1}}}

static void compute_matrices(const App_window& win);

static void prepare_models(const App_window& win)
{
    cube.print();
    compute_matrices(win);
}

static void compute_matrices(const App_window& win)
{
    using glm::vec3;
    // Model
    
    auto pos = vec3(0.0f, 0.0f, 0.0f);
    auto sf = vec3(1.0f, 1.0f, 1.0f);
    auto angle = (GLfloat) 0.0f;
    auto axis = vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 model = glm::mat4(1.0f);	// identity matrix

    model = glm::scale(model, sf);
    model = glm::rotate(model, glm::radians(angle), axis);
    model = glm::translate(model, pos);

    // View
    
    vec3 eye = vec3(8, 3, 5);	 // Camera is at (4,3,3), in World Space
    vec3 center =  vec3(0, 0, 0);	 // and looks at the origin
    vec3 up = vec3(0, 1, 0);	 // Head is up (set to 0,-1,0 to look upside-down)

    glm::mat4 view = glm::lookAt(eye, center, up);

    // Projection
    
    GLfloat fovy =  win.get_fovy() * 3; 
    GLfloat aspect = win.get_aspect();
    GLfloat near = 0.1f;
    GLfloat far = 100.0f;

    glm::mat4 projection =
	glm::perspective(fovy, aspect, near, far);

    // mvp
    
    mvp = projection * view * model;

    // For cubemap

    view = glm::mat4(glm::mat3(view));  

    // vo

    vp = projection * view;
}

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

    //std::cout << "Size : " << sizeof(Vertex_data) << "\n";
    //std::cout << "pos : " << offsetof(Vertex_data, pos) << "\n";
    //std::cout << "normal : " << offsetof(Vertex_data, normal) << "\n";
    //std::cout << "txtr : " << offsetof(Vertex_data, txtr) << "\n";

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_data),
			  (GLvoid *)(offsetof(Vertex_data, pos)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_data),
			  (GLvoid *)(offsetof(Vertex_data, normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_data),
			  (GLvoid *)(offsetof(Vertex_data, txtr)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, skybox_txtr);

    //glBindVertexArray(0);
    // note that following is not needed, the call to glVertexAttribPointer is registered

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

static void prepare_uniforms() {
    vp_id = glGetUniformLocation(skybox_prog, "vp");
    skybox_loc = glGetUniformLocation(skybox_prog, "skybox");
    cubemap_loc = glGetUniformLocation(skybox_prog, "cubemap");
    mvp_id = glGetUniformLocation(cubeobj_prog, "mvp");
}

static void do_draw_commands(const App_window& win)
{

    glBindVertexArray(vao);

    //glActiveTexture(GL_TEXTURE0);

    compute_matrices(win);


    //glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, skybox_txtr);

    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //
    glUseProgram(skybox_prog);
    glUniformMatrix4fv(vp_id, 1, GL_FALSE, &vp[0][0]);
    glUniform1i(skybox_loc, 0);
    glUniform1i(cubemap_loc, 0);
    glDrawElements(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT, (void *)0);

    glUseProgram(cubeobj_prog);
    glUniform1i(skybox_loc, 0);
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
    glDrawElements(GL_TRIANGLES, cube.idx_num, GL_UNSIGNED_SHORT, (void *)0);
    
    //glDepthMask(GL_FALSE);
}

// Fold: static void test_drawing() {{{1
static void test_drawing()
{
    const char *vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 vPos;\n"
	"layout (location = 1) in vec3 vCol;\n"
	"out vec4 fCol;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);\n"
	"   fCol = vec4(vCol.r, vCol.g, vCol.b, 1.0);\n"
	"}\0";

    const char *fragmentShaderSource =
	"#version 330 core\n"
	"in vec4 fCol;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(fCol);\n"
	"}\n\0";

    // build and compile our shader program
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	std::cout << "Vertex shader compilation failed.\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	std::cout << "fragment shader compilation failed.\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	std::cout << "Shade program linking failed.\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes

    // add a new set of vertices to form a second triangle (a total of 6
    // vertices); the vertex attribute configuration remains the same (still one
    // 3-float position vector per vertex)

    // Bad code: position and color are different types

    GLfloat vertices[] = {

	// first triangle

	-0.9f, -0.5f, 0.0f,  // left
	0.5f, 0.2f, 0.0f,    // dark orange
	-0.0f, -0.5f, 0.0f,  // right
	0.5f, 0.2f, 0.0f,    // dark orange
	-0.45f, 0.5f, 0.0f,  // top
	1.0f, 0.4f, 0.0f,    // orange

	// second triangle

	0.0f, -0.5f, 0.0f,  // left
	0.25f, 0.0f, 0.4f,  // dark purple
	0.9f, -0.5f, 0.0f,  // right
	0.25f, 0.0f, 0.4f,  // dark purple
	0.45f, 0.5f, 0.0f,  // top
	0.5f, 0.0f, 0.8f,   // purple

	// third triangle

	-0.45f, 0.5f, 0.0f,   // left
	0.25f, 0.25f, 0.25f,  // dark gray
	0.45f, 0.5f, 0.0f,    // right
	0.25f, 0.25f, 0.25f,  // dark gray
	0.0f, -0.5f, 0.0f,    // bottom
	0.4f, 0.4f, 0.4f,     // gray
    };

    GLuint VBO[1], VAO[1];

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			  (void *)(0 * sizeof(GLfloat)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			  (void *)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered
    // VBO as the vertex attribute's bound vertex buffer object so afterwards we
    // can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other

    // VAOs requires a call to glBindVertexArray anyways so we generally don't
    // unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // sap green background
    glClearColor(0.169f, 0.267f, 0.09f, 0.0f);
    GLFWwindow *window = glfwGetCurrentContext();

    // render loop
    while (!glfwWindowShouldClose(window)) {
	// render
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw our triangles
	glUseProgram(shaderProgram);
	// seeing as we only have a single VAO there's no need to bind it every
	// time, but we'll do so to keep things a bit more organized

	glBindVertexArray(VAO[0]);
	// set the count to 6 since we're drawing 9 vertices now (3 triangles);
	// not 3!

	glDrawArrays(GL_TRIANGLES, 0, 9);
	// no need to unbind it every time

	// glBindVertexArray(0);

	// swap buffers and poll IO events (keys pressed/released, mouse moved
	// etc.)
	glfwSwapBuffers(window);
	glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
    glDeleteProgram(shaderProgram);

    // terminate glfw, clearing all previously allocated GLFW resources.s
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
// 1}}}
