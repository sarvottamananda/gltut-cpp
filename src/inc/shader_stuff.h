// Sarvottamananda (shreesh)
// 2020-09-28
// shader_stuff.h v0.0 (OpenGL Code Snippets)
//
// Shader file loading and compiling

#ifndef SHADER_STUFF_H
#define SHADER_STUFF_H

#include <GL/gl.h>

#include <string>
#include <vector>

GLuint create_program(std::string, std::vector<std::string>);

#endif  // SHADER_STUFF_H
