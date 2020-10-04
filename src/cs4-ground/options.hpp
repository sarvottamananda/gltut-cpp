// Sarvottamananda (shreesh)
// 2020-09-28
// options_store.hpp v0.0 (OpenGL Code Snippets)
//
// Options_store class
//
//     While processing command line arguments, we get
//     1) Meson build configuration parameters
//     2) Environment variables
//     3) Configuration options
//     4) Command line options

#ifndef SNIPPETS_OPTIONS_HPP
#define SNIPPETS_OPTIONS_HPP

#include <string>

class Options {
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
    std::string configdir;

   public:
    Options();
    ~Options() = default;
    // Copy
    Options(const Options&) = default;
    Options& operator=(const Options&) = delete;
    // Move
    Options(Options&&) = delete;
    Options& operator=(Options&&) = delete;
    // Print all the contents, with a title string
    void print(std::string title);
    void process_options(int, char**);
};

#endif	// SNIPPETS_OPTIONS_HPP
