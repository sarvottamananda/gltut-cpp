/*!
 * Sarvottamananda (shreesh)
 * 2020-11-08
 * app_reflection.h v0.0 (OpenGL/C++ Tutorials) 
 *
 * Interface for the derived class
 */

#ifndef APP_REFLECTION_H
#define APP_REFLECTION_H

#include "app_base.h"
#include "options.h"

class App_reflection : public App_base {
   public:
    App_reflection(Window* win) : App_base(win){};
    virtual ~App_reflection() = default;
    void render_loop();

    virtual void key_callback(Key, int, Key_action, Key_mods);
    virtual void initialize(Options& opts);

   private:
};

#endif  // APP_REFLECTION_H
