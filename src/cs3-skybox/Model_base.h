/*
    Sarvottamananda (shreesh)
    2020-09-23
    Model_base.h v0.0 (OpenGL Code Snippets)

    Base Model class
*/

#ifndef MODEL_BASE_H_INCLUDED
#define MODEL_BASE_H_INCLUDED

#include "glad.h"

enum class Model_type { none = 0, point = 1, line = 2, triangle = 3 };

class Model_base {
   public:
    GLfloat* data = nullptr;
    GLuint v_num = 0;
    GLuint* idx = nullptr;
    GLuint idx_num = 0;

    Model_type type;

    GLuint prog;

    void print();
};
#endif	// MODEL_BASE_H_INCLUDED
