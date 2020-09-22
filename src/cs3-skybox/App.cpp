/*
    Sarvottamananda (shreesh)
    2020-09-21
    App.cpp v0.0 (OpenGL Code Snippets)

    Apps derived from App_base
*/

#include "App.h"

#include <array>

#include "Model_cube.h"
#include "img_stuff.h"
#include "iostream"
#include "shader_stuff.h"

using std::string;
template <class T>
using Vector = std::vector<T, std::allocator<T>>;

static GLuint skybox_prog = 0;
static GLuint skybox_txtr = 0;

static int nskyboxes = 0;

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
static void prepare_models();
static void prepare_buffers();
static void prepare_attributes();
static void prepare_uniforms();

static void prepare()
{
    prepare_programs();
    prepare_textures();
    prepare_models();
    prepare_buffers();
    prepare_attributes();
    prepare_uniforms();
}

static void prepare_programs()
{
    Vector<string> shaders = {
	"assets/shaders/skybox.vert",
	"assets/shaders/skybox.frag",
    };

    skybox_prog = create_program("Skybox", shaders);
}

static void load_texture_data();

static void prepare_textures()
{
    glGenTextures(1, &skybox_txtr);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, skybox_txtr);

    load_texture_data();

    // Always set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

// Upload pixel data on textures.
static void splice_texture_data(Vector<Image> &image);

static void load_texture_data()
{
    Vector<string> file = {
	"assets/textures/cubebox-0.png", "assets/textures/cubebox-1.png",
	"assets/textures/cubebox-2.png", "assets/textures/cubebox-3.png",
	"assets/textures/cubebox-4.png", "assets/textures/cubebox-5.png",
	"assets/textures/cubebox-6.png", "assets/textures/cubebox-7.png",
	"assets/textures/cubebox-8.png", "assets/textures/cubebox-9.png",
    };

    nskyboxes = file.size();  // Number of skyboxes

    Vector<Image> image(nskyboxes);

    for (int i = 0; i < nskyboxes; i++) {
	image[i].read_file(file[i]);
    }

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
    int cnt_mip_level = 1;
    glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, cnt_mip_level, GL_RGBA8, fw, fh, nskyboxes);

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
			int pidx = ((fh - jj - 1) * w + ii) * nc + kk + offset;

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

	    glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, i * 6 + j, fw, fh, 1, GL_RGBA,
			    GL_UNSIGNED_BYTE, texels);
	}
    }

    delete[] texels;

    for (int i = 0; i < nskyboxes; i++) {
	image[i].free_data();
    }
}

static void prepare_models() { 
    Model_cube cube;  
    cube.print();
}

static void prepare_buffers() {}

static void prepare_attributes() {}

static void prepare_uniforms() {}
