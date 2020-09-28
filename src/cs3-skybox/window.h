// Sarvottamananda (shreesh)
// 2020-09-28
// window.h v0.0 (OpenGL Code Snippets)
//
// Wrapper for GLFWwindow*

#ifndef SNIPPETS_WINDOW_H
#define SNIPPETS_WINDOW_H

#include <string>

class Window {
   public:
    Window(){};
    virtual ~Window(){};
    virtual bool initialize(std::string title, int width, int height, bool fullscreen) = 0;
    virtual void terminate() = 0;
    virtual void make_current() = 0;
    virtual void render_begin() = 0;
    virtual bool render_cond() = 0;
    virtual void render_end() = 0;
    virtual bool is_valid() = 0;
    virtual float get_aspect() const = 0;
    virtual float get_fovy() const = 0;
};

Window *create_window();
void destroy_window(Window *);

#endif	// SNIPPETS_WINDOW_H
