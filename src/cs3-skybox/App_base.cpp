/*
    Sarvottamananda (shreesh)
    2020-09-20
    App_base.cpp v0.0 (OpenGL Code Snippets)

    Base class for applications
*/
#include "App_base.h"

#include "glad.h"

void App_base::initialize(Opts& os) { 
    w.initialize("Code Snippet 3", os.width, os.height, os.fullscreen); 
}

void App_base::prepare() 
{
    w.render_prepare();
}

void App_base::render_loop() 
{
    while (w.render_cond()) {
        w.render_begin();

	//::glClear(GL_COLOR_BUFFER_BIT);
	w.render_end();
    }
    
}

void App_base::terminate() 
{
    w.terminate();
}
