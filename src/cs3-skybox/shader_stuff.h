// Sarvottamananda (shreesh)
// 2020-09-28
// shader_stuff.h v0.0 (OpenGL Code Snippets)
//
// Shader file loading and compiling

#ifndef SNIPPETS_SHADER_STUFF_H
#define SNIPPETS_SHADER_STUFF_H

#include <GL/glew.h>

#include <string>
#include <vector>
//#include "glad.h"

GLuint create_program(std::string, std::vector<std::string>);

#endif	// SNIPPETS_SHADER_STUFF_H
