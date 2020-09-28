#ifndef SNIPPETS_APP_BASE_H
#define SNIPPETS_APP_BASE_H

//	Sarvottamananda (shreesh)
//	2020-09-28
//	app_base.h v0.0 (OpenGL Code Snippets)
//
//	Base App class

#include <string>

#include "options_store.h"
#include "window.h"

class App_base {
   public:
    App_base() = delete;
    App_base(Window* win) : w(*win){};
    virtual ~App_base() = default;

    virtual void initialize(Options_store& os);
    virtual void render_loop();
    virtual void terminate();

   protected:
    Window& w;

   private:
};

#endif	// SNIPPETS_APP_BASE_H
