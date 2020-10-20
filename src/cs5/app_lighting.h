//	Sarvottamananda (shreesh)
//	2020-09-28
//	app_lighting.h v0.0 (OpenGL Code Snippets)
//
//	Interface for derived App class

#ifndef APP_LIGHTING_H
#define APP_LIGHTING_H

#include "app_base.h"
#include "options.h"

class App_lighting : public App_base {
   public:
    App_lighting(Window* win) : App_base(win){};
    virtual ~App_lighting() = default;
    void render_loop();

    virtual void key_callback(Key, int, Key_action, Key_mods);
    virtual void initialize(Options& opts);

   private:
};

#endif	// APP_LIGHTING_H
