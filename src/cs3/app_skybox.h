//	Sarvottamananda (shreesh)
//	2020-09-28
//	app.h v0.0 (OpenGL Code Snippets)
//
//	Interface for derived App class

#ifndef APP_SKYBOX_H
#define APP_SKYBOX_H

#include "app_base.h"

class App_skybox : public App_base {
   public:
    App_skybox(Window* win) : App_base(win){};
    virtual ~App_skybox() = default;
    void render_loop();

    virtual void key_callback(Key, int, Key_action, Key_mods);

   private:
};

#endif	// APP_SKYBOX_H
