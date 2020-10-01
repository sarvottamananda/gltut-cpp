//	Sarvottamananda (shreesh)
//	2020-09-28
//	img_stuff.hpp v0.0 (OpenGL Code Snippets)
//
//	Image Utility Library

#ifndef SNIPPETS_IMG_STUFF_HPP
#define SNIPPETS_IMG_STUFF_HPP

#include <GL/gl.h>

#include <string>

class Image {
   public:
    void read_file(std::string filename);
    void free_data();
    inline void* pixels() { return data; };
    inline GLsizei get_width() { return width; };
    inline GLsizei get_height() { return height; };
    inline int get_bytes_per_pixel() { return no_channels; };

   private:
    GLsizei width;
    GLsizei height;
    GLenum no_channels;
    void* data;
};

#endif	// SNIPPETS_IMG_STUFF_HPP
