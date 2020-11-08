/*!
 * Sarvottamananda (shreesh)
 * 2020-11-03
 * opengl_stuff.cpp v0.0 (OpenGL/C++ Tutorials)
 *
 * Base App class implementation
 */

#include "opengl_stuff.h"

#include <iostream>

GLenum
check_glerror(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        // clang-format off
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "invalid_enum"; 
                                                   break;
            case GL_INVALID_VALUE:                 error = "invalid_value"; 
                                                   break;
            case GL_INVALID_OPERATION:             error = "invalid_operation"; 
                                                   break;
            case GL_STACK_OVERFLOW:                error = "stack_overflow"; 
                                                   break;
	    case GL_STACK_UNDERFLOW:               error = "stack_underflow"; 
                                                   break;
	    case GL_OUT_OF_MEMORY:                 error = "out_of_memory"; 
                                                   break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "invalid_framebuffer_operation"; 
                                                   break;
            default:                               error = "unknown_gl_error"; 
                                                   break;
        }
        // clang-format on
        std::cerr << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
