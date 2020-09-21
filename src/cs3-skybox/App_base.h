#ifndef APP_BASE_H_INCLUDED
#define APP_BASE_H_INCLUDED

/*
    Sarvottamananda (shreesh)
    2020-09-20
    App_base.h v0.0 (OpenGL Code Snippets)

    Interface for App_base class
*/

#include <string>

#include "Opts.h"
#include "glfw_stuff.h"

class App_base {
   public:
    App_base() = delete;
    App_base(App_window* win) : w(*win){};
    virtual ~App_base() = default;

    void initialize(Opts& os);
    virtual void prepare();
    virtual void render_loop();
    void terminate();

   protected:
    App_window& w;
   private:
};

#endif	// APP_BASE_H_INCLUDED
