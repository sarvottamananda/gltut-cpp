//	Sarvottamananda (shreesh)
//	2020-09-28
//	app_base.hpp v0.0 (OpenGL Code Snippets)
//
//	Base App class

#ifndef SNIPPETS_APP_BASE_HPP
#define SNIPPETS_APP_BASE_HPP

#include <string>

class Window;
class Options;

enum class Key : uint8_t;
enum class Key_action : uint8_t;
enum class Key_mods : uint8_t;

class App_base {
   public:
    App_base() = delete;
    App_base(Window* win) : w(*win){};
    virtual ~App_base() = default;

    virtual void initialize(Options& os);
    virtual void render_loop();
    virtual void terminate();
    virtual void key_callback(Key, int, Key_action, Key_mods);

   protected:
    Window& w;

   private:
};

#endif	// SNIPPETS_APP_BASE_HPP
