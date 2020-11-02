//	Sarvottamananda (shreesh)
//	2020-09-28
//	options_store.cpp v0.0 (OpenGL Code Snippets)
//      Opt class implementation

#include "options_store.h"

#include "cs3_config.h"

Options_store::Options_store()
    : verbose(false),
      debug(0),
      fullscreen(false),
      height(600),
      width(800)
{
    return;
}

void Options_store::print(std::string name)
{
    printf(
	"Options (%s) {\n"
	"\tverbose = %d\n"
	"\tdebug = %d\n"
	"\tfullscreen = %d\n"
	"\theight = %d\n"
	"\twidth = %d\n"
	"}\n",
	name.c_str(), fullscreen, verbose, debug, height, width); 
    fflush(stdout);
}
