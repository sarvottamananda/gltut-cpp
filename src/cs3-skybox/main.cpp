//	Sarvottamananda (shreesh)
//	2020-09-28
//	main.cpp v0.0 (OpenGL Code Snippets)
//
//	Main function for the code snippet.

#include "app.h"
#include "do_args.h"
#include "options_store.h"

int main(int argc, char* argv[])
{
    Options_store opts_store;

    // debug::setdebug(9);
    process_args(argc, argv, opts_store);
    opts_store.print("Options store");

    Window* awin = create_window();
    App code_snippet(awin);

    code_snippet.initialize(opts_store);
    code_snippet.render_loop();
    code_snippet.terminate();

    destroy_window(awin);
}
