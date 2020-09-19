// Project:		OpenGL Code Snippets
// File:		Options_store.h
// Version:		0.0
// Last changed:	2020-09-18
// Purpose:		Code for the debugging system
// Author:		shreesh
// Credits:
// License:		MIT
// Copyright:		(C) Sarvottamananda  2020

#ifndef OPTIONS_STORE_H_INCLUDED
#define OPTIONS_STORE_H_INCLUDED

#include <string>

class Options_store {
public:
    Options_store();

    bool get_verbose() { return verbose; }
    int get_debug() { return debug; }
    bool get_fullscreen() { return fullscreen; }
    int get_height() { return height; }
    int get_width() { return width; }
    std::string get_configfile() { return configfile; }
    std::string get_assetspath() { return assetspath; }
    std::string get_searchpath() { return searchpath; }
    std::string get_modelsdir() { return modelsdir; }
    std::string get_shadersdir() { return shadersdir; }
    std::string get_texturesdir() { return texturesdir; }
    std::string get_builddir() { return builddir; }
    std::string get_sourcedir() { return sourcedir; }

    bool set_verbose(bool x)
    {
	bool oldx = x;
	verbose = x;
	return oldx;
    }
    int set_debug(int x)
    {
	int oldx = x;
	debug = x;
	return oldx;
    }
    bool set_fullscreen(bool x)
    {
	bool oldx = x;
	fullscreen = x;
	return oldx;
    }
    int set_height(int x)
    {
	int oldx = x;
	height = x;
	return oldx;
    }
    int set_width(int x)
    {
	int oldx = x;
	width = x;
	return oldx;
    }
    std::string set_configfile(std::string x)
    {
	std::string oldx = x;
	configfile = x;
	return oldx;
    }
    std::string set_assetspath(std::string x)
    {
	std::string oldx = x;
	assetspath = x;
	return oldx;
    }
    std::string set_searchpath(std::string x)
    {
	std::string oldx = x;
	searchpath = x;
	return oldx;
    }
    std::string set_modelsdir(std::string x)
    {
	std::string oldx = x;
	modelsdir = x;
	return oldx;
    }
    std::string set_shadersdir(std::string x)
    {
	std::string oldx = x;
	shadersdir = x;
	return oldx;
    }
    std::string set_texturesdir(std::string x)
    {
	std::string oldx = x;
	texturesdir = x;
	return oldx;
    }
    std::string set_builddir(std::string x)
    {
	std::string oldx = x;
	builddir = x;
	return oldx;
    }

    std::string set_sourcedir(std::string x)
    {
	std::string oldx = x;
	sourcedir = x;
	return oldx;
    }

    void print(std::string);

private:
    bool verbose;
    int debug;

    bool fullscreen;
    int height;
    int width;

    std::string configfile;
    std::string assetspath;
    std::string searchpath;
    std::string modelsdir;
    std::string shadersdir;
    std::string texturesdir;
    std::string builddir;
    std::string sourcedir;
};

#endif	// end of include guard: OPTIONS_STORE_H_INCLUDED
