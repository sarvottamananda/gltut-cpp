// Sarvottamananda (shreesh)
// 2020-10-04
// model_ground.cpp v0.0 (OpenGL Code Snippets) 
//
// Ground Model

#include "model_ground.h"

#include <iostream>

Model_ground::Model_ground()
{
    static Vertex_data v[] = {
	{{ 0.0f,  0.0f, +0.0f, 1.0f}, {0.0f, 0.0f, +1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}},
	{{+1.0f,  0.0f, +0.0f, 0.0f}, {0.0f, 0.0f, +1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	{{ 0.0f, +1.0f, +0.0f, 0.0f}, {0.0f, 0.0f, +1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	{{-1.0f,  0.0f, +0.0f, 0.0f}, {0.0f, 0.0f, +1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	{{ 0.0f, -1.0f, +0.0f, 0.0f}, {0.0f, 0.0f, +1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    };

    static GLushort vi[] = {
	0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1,
    };

    data = v;
    v_num = 5;
    idx = vi;
    idx_num = 12;

    type = Model_type::triangle;
}
