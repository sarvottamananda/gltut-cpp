// Sarvottamananda (shreesh)
// 2020-09-29
// window_factory.h v0.0 (OpenGL Code Snippets)
//
// Use factory design pattern on windows
//
// We use factory design pattern on window creation, so that we may port the program to other
// platforms.

#ifndef WINDOW_FACTORY_H
#define WINDOW_FACTORY_H

class Window;

enum class Window_type { glfw = 0 };

class Window_factory {
   public:
    static Window* create_window(Window_type);
    static void destroy_window(Window*, Window_type);
};

#endif	// WINDOW_FACTORY_H
