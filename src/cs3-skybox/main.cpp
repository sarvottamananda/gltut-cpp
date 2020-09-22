/*
    Sarvottamananda (shreesh)
    2020-09-20
    main.cpp v0.0 (OpenGL Code Snippets)

    Main function for the code snippet.
*/

#include "App.h"
#include "Opts.h"
#include "do_args.h"

int main(int argc, char* argv[])
{
    Opts opts_store;

    // debug::setdebug(9);
    process_args(argc, argv, opts_store);
    opts_store.print("Options store");

    App_window* awin = create_app_window();
    App code_snippet(awin);

    code_snippet.initialize(opts_store);
    code_snippet.render_loop();
    code_snippet.terminate();

    destroy_app_window(awin);
}
