// Sarvottamananda (shreesh)
// 2020-09-28
// shader_stuff.hpp v0.0 (OpenGL Code Snippets)
//
// Shader file loading and compiling

#ifndef SNIPPETS_SHADER_STUFF_HPP
#define SNIPPETS_SHADER_STUFF_HPP

#include <GL/gl.h>

#include <string>
#include <vector>

GLuint create_program(std::string, std::vector<std::string>);

#endif	// SNIPPETS_SHADER_STUFF_HPP
