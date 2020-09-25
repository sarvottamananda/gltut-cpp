#ifndef APP_WINDOW_H_INCLUDED
#define APP_WINDOW_H_INCLUDED

/*
    Sarvottamananda (shreesh)
    2020-09-21
    App_window.h v0.0 (OpenGL Code Snippets)

    Wrapper for GLFWwindow*
*/

#include <string>

class App_window {
   public:
    App_window(){};
    virtual ~App_window(){};
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

App_window *create_app_window();
void destroy_app_window(App_window *);

#endif	// APP_WINDOW_H_INCLUDED
