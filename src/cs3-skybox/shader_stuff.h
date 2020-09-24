/*
    Sarvottamananda (shreesh)
    2020-09-22
    shader.h v0.0 (OpenGL Code Snippets)

    Shader file loading and compiling
*/

#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <GL/glew.h>

#include <string>
#include <vector>
//#include "glad.h"

GLuint create_program(std::string, std::vector<std::string>);

#endif	// SHADER_H_INCLUDED
