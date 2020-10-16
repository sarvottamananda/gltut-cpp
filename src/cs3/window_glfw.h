// Sarvottamananda (shreesh)
// 2020-09-28
// window_glfw.h v0.0 (OpenGL Code Snippets)
//
// Interface for GLFW3 stuff

#ifndef SNIPPETS_WINDOW_GLFW_H
#define SNIPPETS_WINDOW_GLFW_H

class Window;

/*
typedef void (*App_func0)(const Window&);
typedef void (*App_func1)(const Window&, int);
typedef void (*App_func2)(const Window&, int, int);
typedef void (*App_func3)(const Window&, int, int, int);
typedef void (*App_func4)(const Window&, int, int, int, int);
typedef void (*App_func2f)(const Window&, float, float);
typedef void (*App_func2d)(const Window&, double, double);

void set_window_close_cb(Window&, App_func0);
void set_window_refresh_cb(Window&, App_func0);
void set_window_size_cb(Window&, App_func2);
void set_window_pos_cb(Window&, App_func2);
void set_window_focus_cb(Window&, App_func0);
void set_window_iconfiy_cb(Window&, App_func0);
void set_window_maximize_cb(Window&, App_func0);

void set_framebuffer_size_cb(Window&, App_func2);

void set_window_content_scale_cb(Window&, App_func2d);

void set_key_cb(Window&, App_func4);
void set_char_cb(Window&, App_func1);

void set_mouse_cb(Window&, App_func3);
void set_scroll_cb(Window&, App_func2d);
void set_cur_pos_cb(Window&, App_func2d);
void set_cur_enter_cb(Window&, App_func1);
*/

extern Window* create_glfw_window();
extern void destroy_glfw_window(Window*);

#endif	// SNIPPETS_WINDOW_GLFW_H
