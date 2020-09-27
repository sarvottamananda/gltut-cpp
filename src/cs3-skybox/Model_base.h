/*
    Sarvottamananda (shreesh)
    2020-09-23
    Model_base.h v0.0 (OpenGL Code Snippets)

    Base Model class
*/

#ifndef MODEL_BASE_H_INCLUDED
#define MODEL_BASE_H_INCLUDED

#include <GL/glew.h>
//#include "glad.h"

enum class Model_type { none = 0, point = 1, line = 2, triangle = 3 };

struct Vertex_data {
    GLfloat pos[3];
    GLfloat normal[3];
    GLfloat txtr[2];
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
#endif	// MODEL_BASE_H_INCLUDED
