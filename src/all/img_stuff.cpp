// Sarvottamananda (shreesh)
// 2020-09-28
// img_stuff.cpp v0.0 (OpenGL Code Snippets)
//
// Image Utility Library

#include "img_stuff.h"

#include <stb/stb_image.h>

#include <cassert>
#include <stdexcept>
#include <string>
// #include <iostream>

void
Image::read_file(std::string fname)
{
    int req_nc = 0;	// required number of channels
    int wid = 0;	// image width
    int hgt = 0;	// image height
    int nc = 0;		// number of channels
    void* p = nullptr;	// pixels data

    // std::cerr << "Reading image (" << fname << ")\n";
    p = stbi_load(fname.c_str(), &wid, &hgt, &nc, req_nc);
    if (p == nullptr) {
	throw std::runtime_error("Unable to load image : " + fname);
    }

    assert((nc > 0 && nc <= 4));

    width = wid;
    height = hgt;
    no_channels = nc;
    data = p;
}

void
Image::free_data()
{
    stbi_image_free(data);
}
