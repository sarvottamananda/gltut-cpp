#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

/*
    Sarvottamananda (shreesh)
    2020-09-20
    App.h v0.0 (OpenGL Code Snippets)

    Interface for derived App class
*/

#include "App_base.h"

class App : public App_base {
   public:
    App(App_window* win) : App_base(win){};
    virtual ~App() = default;

    void render_loop();

   private:
};

#endif	// APP_H_INCLUDED
