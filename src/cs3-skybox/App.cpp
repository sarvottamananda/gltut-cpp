/*
    Sarvottamananda (shreesh)
    2020-09-21
    App.cpp v0.0 (OpenGL Code Snippets)

    Apps derived from App_base
*/

#include "App.h"

#include "img_stuff.h"
#include "iostream"
#include "shader_stuff.h"

using std::string;
template <class T>
using Vector = std::vector<T, std::allocator<T>>;

static GLuint skybox_prog = 0;
static GLuint skybox_txtr = 0;

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
    Vector<string> shaders = {
	"assets/shaders/skybox.vert",
	"assets/shaders/skybox.frag",
    };

    skybox_prog = create_program("Skybox", shaders);
}

static void prepare_textures()
{
    Vector<string> file = {
	"assets/textures/cubebox-0.png", "assets/textures/cubebox-1.png",
	"assets/textures/cubebox-2.png", "assets/textures/cubebox-3.png",
	"assets/textures/cubebox-4.png", "assets/textures/cubebox-5.png",
	"assets/textures/cubebox-6.png", "assets/textures/cubebox-7.png",
	"assets/textures/cubebox-8.png", "assets/textures/cubebox-9.png",
    };

    const int nskyboxes = file.size();	// Number of skyboxes

    Vector<Image> image(nskyboxes);

    for (int i = 0; i < nskyboxes; i++) {
	image[i].read_file(file[i]);
    }

    // For a cube map array, all the sizes of the images should be equal. We assume that it is
    // true, and check it later.

    auto w = image[0].get_width();
    auto h = image[0].get_height();
    auto nc = image[0].get_bytes_per_pixel();
    int fw = w / 4;
    int fh = h / 3;

    std::cout << "No. of cubemaps : " << nskyboxes << "\n";
    std::cout << "Cubemaps resolution : " << w << " x " << h << "\n";;
    std::cout << "No. of channels : " << nc << "\n";

    int cnt_mip_level = 1;

    glGenTextures(1, &skybox_txtr);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, skybox_txtr);
    // Allocate the storage.
    //
    glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, cnt_mip_level, GL_RGBA8, fw, fh, nskyboxes);

    // Upload pixel data.
    // The first 0 refers to the mipmap level (level 0, since there's only 1)
    // The following 2 zeroes refers to the x and y offsets in case you only want to specify a
    // subrectangle. The final 0 refers to the layer index offset (we start from index 0 and
    // have 2 levels). Altogether you can specify a 3D box subset of the overall texture, but
    // only one mip level at a time.

    struct Offset {
	int x;
	int y;
    };

    Offset loc_face[6] = {
	Offset{0, 3}, Offset{0, 1}, Offset{1, 2}, Offset{1, 0}, Offset{1, 1}, Offset{1, 3},
    };
    auto texels = new GLubyte[w * h * nc];

    for (int i = 0; i < nskyboxes; i++){
	if (image[i].get_width() != w || image[i].get_height() != h ||
	    image[i].get_bytes_per_pixel() != nc) {
	    std::cerr << "Images do not have same size, resize them equally.\n";
	    exit(EXIT_FAILURE);
	}
	for (int j = 0; j < 6; j++) {
	    GLubyte* p = (GLubyte*)image[i].pixels();

	    int scanlines = (2 - loc_face[j].y) * fh  ;
	    int disp = (loc_face[j].x * fw) ;

            int offset = (scanlines *w +disp) * nc;


	    for (int jj = 0; jj < fh; jj++)
		for (int ii = 0; ii < fw; ii++)
		    for (int kk = 0; kk < nc; kk++){
			int tidx = (fw * jj + ii) * nc + kk ;
                        if (tidx >= fw * fh * nc) 
                            std::cerr << "Boundst : " << tidx << "\n";
                        int pidx = ((fh - jj - 1) * w + ii) * nc + kk + offset;
                        if (pidx >= w * h * nc) 
                            std::cerr << "Boundsp : " << pidx << "\n";
                    }

	    glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, i * 6 + j, fw, fh, 1, GL_RGBA,
			    GL_UNSIGNED_BYTE, texels);
	}
    }

    delete[] texels;

    for (int i = 0; i < nskyboxes; i++) {
	image[i].free_data();
    }

    // Always set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

static void prepare_cube() {}
