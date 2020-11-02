//	Sarvottamananda (shreesh)
//	2020-09-28
//	main.cpp v0.0 (OpenGL Code Snippets)
//
//	Main function for the code snippet.

#include "app_skybox.h"
#include "do_args.h"
#include "options_store.h"
#include "window.h"
#include "window_factory.h"

int
main(int argc, char* argv[])
{
    Options_store opts_store;

    // debug::setdebug(9);
    process_args(argc, argv, opts_store);
    opts_store.print("Options store");

    Window* win_glfw = Window_factory::create_window(
	Window_type::glfw);  // Create a glfw window using factory idiom
    App_skybox code_snippet(
	win_glfw);  // Create an app instance using glfw window, we did not want to
		    // hardcode glfw instance of window inside the app, hence we
		    // create it first and then pass it to app instance
		    //
    win_glfw->set_app(&code_snippet);

    code_snippet.initialize(opts_store);
    code_snippet.render_loop();
    code_snippet.terminate();

    Window_factory::destroy_window(
	win_glfw, Window_type::glfw);  // Destroy the glfw window using factory idiom
}
