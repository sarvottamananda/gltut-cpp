//	Sarvottamananda (shreesh)
//	2020-09-28
//	app_ground.h v0.0 (OpenGL Code Snippets)
//
//	Interface for derived App class

#ifndef APP_GROUND_H
#define APP_GROUND_H

#include "app_base.h"
#include "options.h"

class App_ground : public App_base {
   public:
    App_ground(Window* win) : App_base(win){};
    virtual ~App_ground() = default;
    void render_loop();

    virtual void key_callback(Key, int, Key_action, Key_mods);
    virtual void initialize(Options& opts);

   private:
};

#endif  // APP_GROUND_H
