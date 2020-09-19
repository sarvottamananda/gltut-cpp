// Project:		OpenGL Code Snippets
// File:		Options_store.cpp
// Version:		0.0
// Last changed:	2020-09-19
// Purpose:		Code for the debugging system
// Author:		shreesh
// Credits:
// License:		MIT
// Copyright:		(C) Sarvottamananda  2020

#include "Options_store.h"

#include <iostream>

#include "cs_config.h"

Options_store::Options_store()
    : verbose(false),
      debug(0),
      fullscreen(false),
      height(600),
      width(800),
      configfile(cs_config::cs_config_file),
      assetspath("./assets"),
      searchpath("..:."),
      modelsdir("models"),
      shadersdir("shaders"),
      texturesdir("textures"),
      builddir("."),
      sourcedir(".")
{
    return;
}

void Options_store::print(std::string name)
{
    printf(
	"Option(%s)\n"
	"\tverbose = %d\n"
	"\tdebug = %d\n"
	"\tfullscreen = %d\n"
	"\theight = %d\n"
	"\twidth = %d\n"
	"\tconfigfile = %s\n"
	"\tassetspath = %s\n"
	"\tsearchpath = %s\n"
	"\tmodelsdir = %s\n"
	"\tshadersdir = %s\n"
	"\ttexturesdir = %s\n"
	"\tbuilddir = %s\n"
	"\tsourcedir = %s\n",
	name.c_str(), fullscreen, verbose, debug, height, width, configfile.c_str(),
	assetspath.c_str(), searchpath.c_str(), modelsdir.c_str(), shadersdir.c_str(),
	texturesdir.c_str(), builddir.c_str(), sourcedir.c_str());
}
