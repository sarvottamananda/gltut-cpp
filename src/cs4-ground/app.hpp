//	Sarvottamananda (shreesh)
//	2020-09-28
//	app.hpp v0.0 (OpenGL Code Snippets)
//
//	Interface for derived App class

#ifndef SNIPPETS_APP_HPP
#define SNIPPETS_APP_HPP

#include "app_base.hpp"

class App : public App_base {
   public:
    App(Window* win) : App_base(win){};
    virtual ~App() = default;
    void render_loop();

    virtual void key_callback(Key, int, Key_action, Key_mods);

   private:
};

#endif	// SNIPPETS_APP_HPP
