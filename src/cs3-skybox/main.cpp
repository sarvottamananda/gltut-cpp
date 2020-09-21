/*
    Sarvottamananda (shreesh)
    2020-09-20
    main.cpp v0.0 (OpenGL Code Snippets)

    Main function for the code snippet.
*/

#include "App.h"
#include "Opts.h"
#include "debug.h"
#include "do_args.h"

void initialize_snippet(App_base& ab, Opts& os);
void run_snippet(App_base& ab, Opts&);
void terminate_snippet(App_base& ab, Opts&);

int main(int argc, char* argv[])
{
    Opts opts_store;

    // debug::setdebug(9);
    process_args(argc, argv, opts_store);
    opts_store.print("Options store");

    App_window* awin = create_app_window();
    App code_snippet(awin);

    initialize_snippet(code_snippet, opts_store);
    run_snippet(code_snippet, opts_store);
    terminate_snippet(code_snippet, opts_store);

    destroy_app_window(awin);
}

void initialize_snippet(App_base& ab, Opts& os) { ab.initialize(os); }

void run_snippet(App_base& ab, Opts& os)
{
    ab.prepare();
    ab.render_loop();
}

void terminate_snippet(App_base& ab, Opts& os) { ab.terminate(); }
