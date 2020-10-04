//	Sarvottamananda (shreesh)
//	2020-09-28
//	model_base.hpp v0.0 (OpenGL Code Snippets)
//
//      Base Model class

#ifndef SNIPPETS_MODEL_BASE_HPP
#define SNIPPETS_MODEL_BASE_HPP

#include <GL/gl.h>

enum class Model_type { none = 0, point = 1, line = 2, triangle = 3 };

struct Vertex_data {
    GLfloat pos[4];
    GLfloat normal[4];
    GLfloat txtr[4];
};

class Model_base {
   public:
    Vertex_data* data = nullptr;
    GLsizei v_num = 0;
    GLushort* idx = nullptr;
    GLsizei idx_num = 0;

    Model_type type;

    GLuint prog;

    void print();
};
#endif	// SNIPPETS_MODEL_BASE_HPP
