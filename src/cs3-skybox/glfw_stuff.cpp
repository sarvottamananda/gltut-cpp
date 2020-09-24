/*
    Sarvottamananda (shreesh)
    2020-09-20
    glfw_stuff.cpp v0.0 (OpenGL Code Snippets)

    GLFW3 stuff

    All GLFW3 library stuff should go here. We should be sure not to include glfw.h elsewhere
    for portability and multi-os support.

*/
#include "glfw_stuff.h"

#include <iostream>
// clang-format off
#include <GL/glew.h>
//#include "glad.h"  // glad is written in C
#include <GLFW/glfw3.h>	 // GLFW3 is written in C
// clang-format on

using std::cerr;

constexpr float view_distance = 2016.0f; // Viewing distance in pixels

static    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

class App_window_impl : public App_window {
   public:
    App_window_impl() : App_window(), glfw_window(nullptr){};
    virtual ~App_window_impl(){};

    bool initialize(std::string title, int width, int height, bool fullscreen);
    void terminate();
    void make_current();
    void render_begin();
    bool render_cond();
    void render_end();
    bool is_valid();

    float get_aspect() const {return aspect;};
    float get_fovy() const {return fovy;};

   private:
    ::GLFWwindow *glfw_window;
    int width;
    int height;
    float aspect;
    float fovy;

    friend void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};

App_window *create_app_window() { return new App_window_impl; }
void destroy_app_window(App_window *aw) { delete aw; }

static void error_callback(int err, const char *desc);
static void framebuffer_size_callback(::GLFWwindow *window, int width, int height);

bool App_window_impl::initialize(std::string title, int width, int height, bool fullscreen)
{
    if (!glfwInit()) {
	cerr << "Initialization failed (GLFW3)\n";
	exit(EXIT_FAILURE);
    }

    ::glfwSetErrorCallback(error_callback);

    ::glfwWindowHint(GLFW_SAMPLES, 4);
    // We need core profile 4.0 because of cube map array
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    ::glfwWindowHint(GLFW_DEPTH_BITS, 32);
    ::glfwWindowHint(GLFW_STENCIL_BITS, 32);
    ::glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, 1 );
    // For Apple, but sadly Apple is removing OpenGL from it OSes in favour of Metal API

    // ::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

	/*
	if (width == 0) width = pwidth;
	else width = (width < 640 ? 640 : width);

	if (height == 0) height = pheight;
	else height = (height < 360 ? 360 : height);

	window = ::glfwCreateWindow(width, height, title.c_str(), pmonitor, NULL);
	*/

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

    ::glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));
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

    glfw_window = window;
    glfwGetWindowSize(window, &(this->width), &(this->height));  

    aspect = (float)width/(float)height;

    fovy =
	height / view_distance;  // assuming we are viewing the monitor at a distance twice
				    // the height of monitor and it has resolution 1920x1080

    return true;
}

void error_callback(int error, const char *description)
{
    cerr << "GLFW error (" << error << ") " << description << "\n";
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void App_window_impl::make_current()
{
    // This makes the window context, current OpenGL context for rendering
    ::glfwMakeContextCurrent(glfw_window);

    if (glfwGetCurrentContext() != glfw_window) {
	cerr << "Window context not set.\n";
	exit(EXIT_FAILURE);
    }
}

void App_window_impl::render_begin()
{
    // Poll for the events
    ::glfwPollEvents();
    ::glfwWaitEvents();
}

bool App_window_impl::render_cond()
{
    // Check if the ESC key was pressed or the window was closed
    auto render_condition = (::glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			     ::glfwWindowShouldClose(glfw_window) == 0);

    return render_condition;
}

void App_window_impl::render_end()
{
    // Swap buffers
    ::glfwSwapBuffers(glfw_window);
}

void App_window_impl::terminate() { glfwTerminate(); }

bool App_window_impl::is_valid()
{
    if (glfw_window != nullptr) {
	std::cout << "Current window is a valid context (" << (void *)glfw_window << ")\n";
	return true;
    }
    return false;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    App_window_impl * handler = reinterpret_cast<App_window_impl *>(glfwGetWindowUserPointer(window));
    if (handler) {
	handler->aspect = (float)width / (float)height;
	handler->fovy = (float)height / view_distance;
    }
    
    glViewport(0, 0, width, height);
}
