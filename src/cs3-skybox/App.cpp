/*
    Sarvottamananda (shreesh)
    2020-09-21
    App.cpp v0.0 (OpenGL Code Snippets)

    Apps derived from App_base
*/

#include "App.h"

#include "shader_stuff.h"

using std::string;
template <class T>
using Vector = std::vector<T, std::allocator<T>>;

static void prepare();

void App::render_loop()
{
    // This makes w's OpenGL context current
    w.make_current();

    // We need the context to compile programs and such
    prepare();

    while (w.render_cond()) {
	w.render_begin();

	//::glClear(GL_COLOR_BUFFER_BIT);
	//
	w.render_end();
    }
}

static void prepare_programs();
static void prepare_textures();
static void prepare_cube();

static void prepare()
{
    prepare_programs();
    prepare_textures();
    prepare_cube();
}

static void prepare_programs()
{
    Vector<string> shader_list = {
	"assets/shaders/skybox.vert",
	"assets/shaders/skybox.frag",
    };

    GLuint skybox_prog = create_program("Skybox", shader_list);
}

static void prepare_textures() {}

static void prepare_cube() {}
