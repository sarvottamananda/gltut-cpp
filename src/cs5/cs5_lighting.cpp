/*!
 * Sarvottamananda (shreesh)
 * 2020-11-03
 * cs5_lighting.cpp v0.0 (OpenGL/C++ Tutorials)
 *
 * Main function for the code snippet.
 */

#include "app_lighting.h"
#include "cs5_config.h"
#include "debug.h"
#include "options.h"
#include "window.h"
#include "window_factory.h"

static void process_buildconf(Options& op);

int
main(int argc, char* argv[])
{
    Options opts;

    // debug::setdebug(9);
    opts.process_options(argc, argv);
    opts.print("Options store");

    // Create a glfw window using factory idiom
    Window* win_glfw = Window_factory::create_window(Window_type::glfw);

    // Create an app instance using glfw window, we did not want to
    // hardcode glfw instance of window inside the app, hence we
    // create it first and then pass it to app instance
    App_lighting code_snippet(win_glfw);

    win_glfw->set_app(&code_snippet);

    code_snippet.initialize(opts);
    code_snippet.render_loop();
    code_snippet.terminate();

    Window_factory::destroy_window(
        win_glfw, Window_type::glfw);  // Destroy the glfw window using factory idiom
}
