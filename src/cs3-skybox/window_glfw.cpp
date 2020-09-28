//	Sarvottamananda (shreesh)
//	2020-09-28
//	window_glfw.cpp v0.0 (OpenGL Code Snippets)
//
//	GLFW3 stuff
//
//      All GLFW3 library stuff should go here. We should be sure not to include glfw.h
//      elsewhere for portability and multi-os support.

#include "window_glfw.h"

#include <iostream>

// clang-format off
#include <GL/glew.h>
//#include "glad.h"  // glad is written in C
#include <GLFW/glfw3.h>	 // GLFW3 is written in C
// clang-format on

// using definitions
using std::cerr;

// unnamed namespace
namespace {
constexpr float view_distance = 2016.0f;  // Viewing distance in pixels

// Implementation class that hides glfw window datastructure

class Window_glfw : public Window {
   public:
    Window_glfw() : Window(), window(nullptr){};
    virtual ~Window_glfw(){};

    bool initialize(std::string title, int width, int height, bool fullscreen);
    void terminate();
    void make_current();
    void render_begin();
    bool render_cond();
    void render_end();
    bool is_valid();

    float get_aspect() const { return aspect; };
    float get_fovy() const { return fovy; };

   private:
    ::GLFWwindow *window;
    int width;
    int height;
    float aspect;
    float fovy;

    friend void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    Window_glfw *handler = static_cast<Window_glfw *>(glfwGetWindowUserPointer(window));
    if (handler) {
	handler->aspect = (float)width / (float)height;
	handler->fovy = (float)height / view_distance;
    }

    glViewport(0, 0, width, height);
}

void error_callback(int error, const char *description)
{
    cerr << "GLFW error (" << error << ") " << description << "\n";
}

}  // namespace

// helper functions

// The extern functions needed to access and destroy the implementation app window hidden in
// unnamed namespace above.

Window *create_window() { return new Window_glfw; }
void destroy_window(Window *aw) { delete aw; }

// All the overridden virtual functions defined in sequence. Later on if we need to override
// them we shall make them virtual. But right now there is no need to bother with mixing virtual
// and non-virtual functions.

// bool Window_glfw::initialize(std::string title, int width, int height, bool fullscreen)
// {{{1
bool Window_glfw::initialize(std::string title, int width, int height, bool fullscreen)
// We initialize both glfw and glew, and open a window which is either windowed or fullscreen,
// depending on the fullscreen parameter. Opening of window/fullscreen is necessary because glew
// needs a context to get initialized.
{
    if (!glfwInit()) {
	cerr << "Initialization failed (GLFW3)\n";
	exit(EXIT_FAILURE);
    }

    ::glfwSetErrorCallback(error_callback);

    // We need core profile 4.0 because of cube map array
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    ::glfwWindowHint(GLFW_SAMPLES, 4);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    ::glfwWindowHint(GLFW_DEPTH_BITS, 32);
    ::glfwWindowHint(GLFW_STENCIL_BITS, 32);
    ::glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
    // For Apple, but sadly Apple is removing OpenGL from it OSes in favour of Metal API, but it
    // is good to deprecate the deprecated API.
    ::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    ::GLFWwindow *window = NULL;

    if (title.empty()) title = "Code Snippet";

    if (fullscreen) {
	int count = 0;
	::GLFWmonitor *pmonitor = ::glfwGetPrimaryMonitor();
	const ::GLFWvidmode *pvidmodes = ::glfwGetVideoModes(pmonitor, &count);

	auto max_pixels = 0;
	auto max_mode = 0;
	auto max_hz = 0;

	for (int i = 0; i < count; i++) {
	    if (pvidmodes[i].height * pvidmodes[i].width > max_pixels) {
		max_pixels = pvidmodes[i].width * pvidmodes[i].height;
		max_hz = pvidmodes[i].refreshRate;
		max_mode = i;
	    }
	    if (pvidmodes[i].height * pvidmodes[i].width == max_pixels) {
		if (pvidmodes[i].refreshRate > max_hz) {
		    max_pixels = pvidmodes[i].width * pvidmodes[i].height;
		    max_hz = pvidmodes[i].refreshRate;
		    max_mode = i;
		}
	    }

	    // Hopefully we have got the best resolution with best refresh rate

	    printf("No:%d, %dx%d, RGB:(%d %d %d)i @ %dHz\n", i, pvidmodes[i].width,
		   pvidmodes[i].height, pvidmodes[i].redBits, pvidmodes[i].greenBits,
		   pvidmodes[i].blueBits, pvidmodes[i].refreshRate);
	}

	// Hack to get fullscreen with best resolution
	// We select the one with maximum pixels

	int pwidth = pvidmodes[max_mode].width;
	int pheight = pvidmodes[max_mode].height;
	int predbits = pvidmodes[max_mode].redBits;
	int pgreenbits = pvidmodes[max_mode].greenBits;
	int pbluebits = pvidmodes[max_mode].blueBits;
	int prefreshrate = pvidmodes[max_mode].refreshRate;

	printf("Mode selected :%d, %dx%d@%dHz, RGB(%d,%d,%d)\n", max_mode, pwidth, pheight,
	       prefreshrate, predbits, pgreenbits, pbluebits);

	// Next, since we are forced to, we try with the supplied resolution, if we fail to
	// create a fullscreen window,
	// we fail silently and ungracefully

	// if (width == 0) width = pwidth;
	// else width = (width < 640 ? 640 : width);
	//
	// if (height == 0) height = pheight;
	// else height = (height < 360 ? 360 : height);
	//
	// window = ::glfwCreateWindow(width, height, title.c_str(), pmonitor, NULL);

	window = ::glfwCreateWindow(pwidth, pheight, title.c_str(), pmonitor, NULL);

	const ::GLFWvidmode *pvidmode = ::glfwGetVideoMode(pmonitor);

	printf("Started fullscreen %d x %d, RGB:(%d:%d:%d), Rate:%dHz\n\n", pvidmode->width,
	       pvidmode->height, pvidmode->redBits, pvidmode->greenBits, pvidmode->blueBits,
	       pvidmode->refreshRate);
    }
    else {
	if (width == 0) width = 640;
	width = (width < 640 ? 640 : width);
	if (height == 0) height = 480;
	height = (height < 360 ? 360 : height);
	window = ::glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	int w, h;
	::glfwGetWindowSize(window, &w, &h);
	printf("Started window %d x %d\n\n", w, h);
    }

    if (window == NULL) {
	cerr << "Failed to open GLFW window.\n";
	::glfwTerminate();
	exit(EXIT_FAILURE);
    }

    // Ensure we can capture the keys and mouse button being pressed and
    // released before polling
    ::glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    ::glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

    ::glfwSetWindowUserPointer(window, static_cast<void *>(this));
    ::glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad stuff did not work first try so fallback to glew

    // glad: load all OpenGL function pointers
    // ---------------------------------------

    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    //	std::cerr << "Failed to initialize GLAD" << std::endl;
    //	exit(EXIT_FAILURE);
    //}

    // glew stuff

    ::glfwMakeContextCurrent(window);
    cerr << "Initializing glew"
	 << "\n";
    // initialize glew
    if (glewInit() != GLEW_OK) {
	cerr << "Failed to initialize glew.\n";
	glfwTerminate();
	exit(EXIT_FAILURE);
    }

    ::glfwSwapInterval(1);

    this->window = window;
    glfwGetWindowSize(window, &(this->width), &(this->height));

    aspect = (float)width / (float)height;

    fovy = height / view_distance;  // assuming we are viewing the monitor at a distance twice
				    // the height of monitor and it has resolution 1920x1080

    return true;
}
// 1}}}

void Window_glfw::make_current()
{
    //// This makes the window context, current OpenGL context for rendering
    //::glfwMakeContextCurrent(window);

    if (glfwGetCurrentContext() != window) {
	cerr << "Window context not set.\n";
	exit(EXIT_FAILURE);
    }
}

void Window_glfw::render_begin()
{
    // Poll for the events
    ::glfwPollEvents();
}

bool Window_glfw::render_cond()
{
    // Check if the ESC key was pressed or the window was closed
    auto render_condition = (::glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			     ::glfwWindowShouldClose(window) == 0);
    return render_condition;
}

void Window_glfw::render_end()
{
    // Swap buffers
    ::glfwSwapBuffers(window);
    // And then idle wait
    ::glfwWaitEvents();
}

void Window_glfw::terminate() { glfwTerminate(); }

bool Window_glfw::is_valid()
{
    if (window != nullptr) {
	std::cout << "Current window is a valid context (" << (void *)window << ")\n";
	return true;
    }
    return false;
}
