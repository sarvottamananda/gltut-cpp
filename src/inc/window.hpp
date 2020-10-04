// Sarvottamananda (shreesh)
// 2020-09-28
// window.hpp v0.0 (OpenGL Code Snippets)
//
// Wrapper for GLFWwindow*

#ifndef SNIPPETS_WINDOW_HPP
#define SNIPPETS_WINDOW_HPP

#include <string>

class App_base;

class Window {
   public:
    Window(){};
    virtual ~Window(){};
    virtual bool initialize(std::string title, int width, int height, bool fullscreen) = 0;
    virtual void terminate() = 0;
    virtual void make_current() = 0;
    virtual void render_begin() = 0;
    virtual bool render_cond() = 0;
    virtual void render_end() = 0;
    virtual bool is_valid() = 0;
    virtual float get_aspect() const = 0;
    virtual float get_fovy() const = 0;

    inline void set_app(App_base* app) { this->app = app; };

   protected:
    App_base* app;
};

enum class Key : uint8_t {
    none = 0,
    left_shift = 1,
    right_shift = 2,
    left_ctrl = 3,
    right_ctrl = 4,
    left_alt = 5,
    right_alt = 6,
    left_super = 7,
    right_super = 8,
    left = 9,
    right = 10,
    up = 11,
    down = 12,
    pgup = 13,
    pgdn = 14,
    home = 15,
    end = 16,
    ins = 17,
    del = 18,
    space = 19,
    esc = 20,

    k0 = 21,
    k1 = 22,
    k2 = 23,
    k3 = 24,
    k4 = 25,
    k5 = 26,
    k6 = 27,
    k7 = 28,
    k8 = 29,
    k9 = 30,

    kw = 31,
    ks = 32,
    ka = 33,
    kd = 34,

    caps_lock = 31,
    scroll_lock = 32,
    num_lock = 33,

    unknown = 63,
    max,
};

enum class Key_action : uint8_t {
    none = 0,
    release = 1,
    press = 2,
    repeat = 3,
    max,
};

// Define the keypress modifiers same as GLFW to save mapping, if we change here, we also need
// to change in window_glfw.cpp file.

enum class Key_mods : uint8_t {
    none = 0,
    shift = 1 << 0,
    ctrl = 1 << 1,
    alt = 1 << 2,
    super = 1 << 3,
    caps_lock = 1 << 4,
    num_lock = 1 << 5,
};

#endif	// SNIPPETS_WINDOW_HPP
