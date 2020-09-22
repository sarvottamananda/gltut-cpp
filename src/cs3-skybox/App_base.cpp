/*
    Sarvottamananda (shreesh)
    2020-09-20
    App_base.cpp v0.0 (OpenGL Code Snippets)

    Base class for applications
*/
#include "App_base.h"

#include "glad.h"

void App_base::initialize(Opts& os)
{
    // Opens a window of required resolution and mode
    w.initialize("Code Snippet 3", os.width, os.height, os.fullscreen);
}

void App_base::render_loop()
{
    // Makes the window the current  OpenGl context for rendering
    w.make_current();
    while (w.render_cond()) {
	// Just polls events
	w.render_begin();

	// Just swap buffers
	w.render_end();
    }
}

void App_base::terminate() { w.terminate(); }
