//	Sarvottamananda (shreesh)
//	2020-09-28
//	main.cpp v0.0 (OpenGL Code Snippets)
//
//	Main function for the code snippet.

#include "app.h"
#include "options.h"
#include "window.h"
#include "window_factory.h"

int
main(int argc, char* argv[])
{
    Options opts;

    // debug::setdebug(9);
    opts.process_options(argc, argv);
    opts.print("Options store");

    Window* win_glfw = Window_factory::create_window(
	Window_type::glfw);	 // Create a glfw window using factory idiom
    App code_snippet(win_glfw);	 // Create an app instance using glfw window, we did not want to
				 // hardcode glfw instance of window inside the app, hence we
				 // create it first and then pass it to app instance

    win_glfw->set_app(&code_snippet);

    code_snippet.initialize(opts);
    code_snippet.render_loop();
    code_snippet.terminate();

    Window_factory::destroy_window(
	win_glfw, Window_type::glfw);  // Destroy the glfw window using factory idiom
}
