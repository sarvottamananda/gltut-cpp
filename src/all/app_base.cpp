//	Sarvottamananda (shreesh)
//	2020-09-28
//	app_base.cpp v0.0 (OpenGL Code Snippets)
//
//	Base App class implementation

#include "app_base.hpp"

#include "window.hpp"
#include <iostream>

void App_base::initialize(Options& os)
{
    // Opens a window of required resolution and mode
    w.initialize("OpenGL Snippets : Basic App", 800, 600, false);
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
void App_base::key_callback(Key k, int sc, Key_action ka, Key_mods km) { return; }

