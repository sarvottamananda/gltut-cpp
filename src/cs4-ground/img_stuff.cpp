// Sarvottamananda (shreesh)
// 2020-09-28
// img_stuff.cpp v0.0 (OpenGL Code Snippets)
//
// Image Utility Library

#include "img_stuff.hpp"

#include <stb/stb_image.h>

#include <iostream>
#include <string>

void Image::read_file(std::string fname)
{
    int wid = 0;
    int hgt = 0;
    int nc = 0;
    void* p = nullptr;

    std::cerr << "Reading image (" << fname << ")\n";

    p = stbi_load(fname.c_str(), &wid, &hgt, &nc, 0);

    if (p == nullptr) {
	std::cerr << "Unable to load image (" << fname << ")\n";
	exit(EXIT_FAILURE);
    }

    width = wid;
    height = hgt;
    if (nc > 0 && nc <= 4) {
	no_channels = nc;
    }
    else {
	std::cerr << "Invalid no. of channels (" << nc << ")\n";
	exit(EXIT_FAILURE);
    }
    data = p;
}

void Image::free_data() { stbi_image_free(data); }
