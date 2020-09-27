#ifndef DO_ARGS_H_INCLUDED
#define DO_ARGS_H_INCLUDED

/*
    Sarvottamananda (shreesh)
    2020-09-20
    do_args.h v0.0 (OpenGL Code Snippets)

    Interface to get command line arguments

    While processing command line arguments, we get
    1) Meson build configuration parameters
    2) Environment variables
    3) Configuration options
    4) Command line options
*/

#include "Opts.h"

void process_args(int, char**, Opts& os);

#endif	// DO_ARGS_H_INCLUDED
