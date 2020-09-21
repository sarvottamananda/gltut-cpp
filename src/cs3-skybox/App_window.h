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
    virtual void render_prepare() = 0;
    virtual void render_begin() = 0;
    virtual bool render_cond() = 0;
    virtual void render_end() = 0;
};

App_window *create_app_window();
void destroy_app_window(App_window *);

#endif	// APP_WINDOW_H_INCLUDED