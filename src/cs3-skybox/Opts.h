#ifndef OPTS_H_INCLUDED
#define OPTS_H_INCLUDED

/*
    Sarvottamananda (shreesh)
    2020-09-20
    Opts.h v0.0 (OpenGL Code Snippets)

    Interface for Options store that can be accessed globally
*/

#include <string>

class Opts {
   public:
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
    std::string homedir;

   public:
    Opts();
    ~Opts() = default;
    // Copy
    Opts(const Opts&) = default;
    Opts& operator=(const Opts&) = delete;
    // Move
    Opts(Opts&&) = delete;
    Opts& operator=(Opts&&) = delete;
    // Print all the contents, with a title string
    void print(std::string title);
};

#endif	// OPTS_H_INCLUDED
