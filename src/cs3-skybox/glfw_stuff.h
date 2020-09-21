#ifndef GLFW_STUFF_H_INCLUDED
#define GLFW_STUFF_H_INCLUDED

/*
    Sarvottamananda (shreesh)
    2020-09-20
    glfw_stuff.h v0.0 (OpenGL Code Snippets)

    Interface for GLFW3 stuff
*/

#include "App_window.h"

typedef void (*App_func0)(const App_window&);
typedef void (*App_func1)(const App_window&, int);
typedef void (*App_func2)(const App_window&, int, int);
typedef void (*App_func3)(const App_window&, int, int, int);
typedef void (*App_func4)(const App_window&, int, int, int, int);

typedef void (*App_func2f)(const App_window&, float, float);
typedef void (*App_func2d)(const App_window&, double, double);

void set_window_close_cb(App_window&, App_func0);
void set_window_refresh_cb(App_window&, App_func0);
void set_window_size_cb(App_window&, App_func2);
void set_window_pos_cb(App_window&, App_func2);
void set_window_focus_cb(App_window&, App_func0);
void set_window_iconfiy_cb(App_window&, App_func0);
void set_window_maximize_cb(App_window&, App_func0);

void set_framebuffer_size_cb(App_window&, App_func2);

void set_window_content_scale_cb(App_window&, App_func2d);

void set_key_cb(App_window&, App_func4);
void set_char_cb(App_window&, App_func1);

void set_mouse_cb(App_window&, App_func3);
void set_scroll_cb(App_window&, App_func2d);
void set_cur_pos_cb(App_window&, App_func2d);
void set_cur_enter_cb(App_window&, App_func1);

#endif	// GLFW_STUFF_H_INCLUDED
