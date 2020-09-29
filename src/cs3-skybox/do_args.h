#ifndef SNIPPETS_DO_ARGS_H
#define SNIPPETS_DO_ARGS_H

//	Sarvottamananda (shreesh)
//	2020-09-28
//	do_args.h v0.0 (OpenGL Code Snippets)
//
//      Interface to get command line arguments
//
//     While processing command line arguments, we get
//     1) Meson build configuration parameters
//     2) Environment variables
//     3) Configuration options
//     4) Command line options

class Options_store;

void process_args(int, char**, Options_store& os);

#endif	// SNIPPETS_DO_ARGS_H
