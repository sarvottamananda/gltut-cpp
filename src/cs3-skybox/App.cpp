/*
    Sarvottamananda (shreesh)
    2020-09-21
    App.cpp v0.0 (OpenGL Code Snippets)

    Apps derived from App_base
*/

#include "App.h"

#include "glad.h"

void App::initialize(Opts& os) { 
    w.initialize("Code Snippet 3", os.width, os.height, os.fullscreen); 
}

void App::prepare() 
{
    w.render_prepare();
}

void App::render_loop() 
{
    while (w.render_cond()) {
        w.render_begin();

	//::glClear(GL_COLOR_BUFFER_BIT);
	w.render_end();
    }
    
}

void App::terminate() 
{
    w.terminate();
}
