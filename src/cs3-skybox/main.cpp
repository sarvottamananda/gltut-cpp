// Project:		OpenGL Code Snippets
// File:		main.cpp
// Version:		0.0
// Last changed:	2020-09-19
// Purpose:		Code for the debugging system
// Author:		shreesh
// Credits:
// License:		MIT
// Copyright:		(C) Sarvottamananda  2020

#include "Options_store.h"
#include "debug.h"
#include "do_args.h"

Options_store a_store;

int main(int argc, char* argv[])
{
    debug::setdebug(9);
    process_args(argc, argv, a_store);
    a_store.print("Inited store");
}
