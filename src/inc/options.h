// Sarvottamananda (shreesh)
// 2020-09-28
// options.h v0.0 (OpenGL Code Snippets)
//
// Options class
//
//     While processing command line arguments, we get
//     1) Meson build configuration parameters
//     2) Environment variables
//     3) Configuration options
//     4) Command line options

#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>

class Options {
   public:
    Options();
    ~Options() = default;
    // Copy
    Options(const Options&) = default;
    Options& operator=(const Options&) = delete;
    // Move
    Options(Options&&) = delete;
    Options& operator=(Options&&) = delete;

    bool verbose;
    int debug;

    bool fullscreen;
    int height;
    int width;

    // Print all the contents, with a title string
    void print(std::string title);
    void process_options(int, char**);
};

#endif	// OPTIONS_H
