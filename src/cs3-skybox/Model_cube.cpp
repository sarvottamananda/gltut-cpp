/*
    Sarvottamananda (shreesh)
    2020-09-22
    model_cube.cpp v0.0 (OpenGL Code Snippets)

    Cube Model
*/

#include "Model_cube.h"

#include <iostream>

Model_cube::Model_cube()
{
    static Vertex_data v[] = {
	/*
	{{-0.5f, -0.5f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{+0.5f, -0.5f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{+0.0f, 0.5f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	*/

	{{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{-1.0f, +1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{-1.0f, +1.0f, +1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	{{-1.0f, -1.0f, +1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

	{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
	{{-1.0f, -1.0f, +1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
	{{+1.0f, -1.0f, +1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
	{{+1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},

	{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
	{{+1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
	{{+1.0f, +1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
	{{-1.0f, +1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},

	{{+1.0f, -1.0f, -1.0f}, {+1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{+1.0f, -1.0f, +1.0f}, {+1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{+1.0f, +1.0f, +1.0f}, {+1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	{{+1.0f, +1.0f, -1.0f}, {+1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

	{{-1.0f, +1.0f, -1.0f}, {0.0f, +1.0f, 0.0f}, {0.0f, 0.0f}},
	{{+1.0f, +1.0f, -1.0f}, {0.0f, +1.0f, 0.0f}, {0.0f, 1.0f}},
	{{+1.0f, +1.0f, +1.0f}, {0.0f, +1.0f, 0.0f}, {1.0f, 1.0f}},
	{{-1.0f, +1.0f, +1.0f}, {0.0f, +1.0f, 0.0f}, {1.0f, 0.0f}},

	{{-1.0f, -1.0f, +1.0f}, {0.0f, 0.0f, +1.0f}, {0.0f, 0.0f}},
	{{-1.0f, +1.0f, +1.0f}, {0.0f, 0.0f, +1.0f}, {0.0f, 1.0f}},
	{{+1.0f, +1.0f, +1.0f}, {0.0f, 0.0f, +1.0f}, {1.0f, 1.0f}},
	{{+1.0f, -1.0f, +1.0f}, {0.0f, 0.0f, +1.0f}, {1.0f, 0.0f}},
    };

    static GLushort vi[] = {
	0,  1,	2,  2,	3,  0,	4,  5,	6,  6,	7,  4,	8,  9,	10, 10, 11, 8,
	12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
    };

    data = v;
    v_num = 24;
    idx = vi;
    idx_num = 36;

    type = Model_type::triangle;
}
