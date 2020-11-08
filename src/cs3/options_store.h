// Sarvottamananda (shreesh)
// 2020-09-28
// options_store.h v0.0 (OpenGL Code Snippets)
//
// Options_store class

#ifndef SNIPPETS_OPTIONS_STORE_H
#define SNIPPETS_OPTIONS_STORE_H

#include <string>

class Options_store {
   public:
    bool verbose;
    int debug;

    bool fullscreen;
    int height;
    int width;

   public:
    Options_store();
    ~Options_store() = default;
    // Copy
    Options_store(const Options_store&) = default;
    Options_store& operator=(const Options_store&) = delete;
    // Move
    Options_store(Options_store&&) = delete;
    Options_store& operator=(Options_store&&) = delete;
    // Print all the contents, with a title string
    void print(std::string title);
};

#endif  // SNIPPETS_OPTIONS_STORE_H
