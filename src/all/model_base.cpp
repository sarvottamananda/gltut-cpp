// Sarvottamananda (shreesh)
// 2020-09-28
// model_base.cpp v0.0 (OpenGL Code Snippets)
//
// Base Model class

#include "model_base.h"

#include <iostream>

void Model_base::print()
{
    using std::cout;

    cout << "Model_base :\n";
    cout << v_num << " vertices, " << idx_num << " indices\n";

    cout << "    Vertices:\n";

    for (auto i = 0; i < v_num; i++) {
	cout << "\t(";
	for (auto j = 0; j < 4; j++) {
	    cout << data[i].pos[j];
	    if (j == 3)
		cout << ") ";
	    else
		cout << ", ";
	}

	cout << "(";
	for (auto j = 0; j < 4; j++) {
	    cout << data[i].normal[j];
	    if (j == 3)
		cout << ") ";
	    else
		cout << ", ";
	}

	cout << "(";
	for (auto j = 0; j < 4; j++) {
	    cout << data[i].tx[0];
	    if (j == 3)
		cout << ")\n";
	    else
		cout << ", ";
	}
    }

    cout << "    Indices:\n";
    for (auto i = 0; i < idx_num / 3; i++) {
	cout << "\t(";
	for (auto j = 0; j < 3; j++) {
	    cout << idx[i * 3 + j];
	    if (j == 2)
		cout << ")\n";
	    else
		cout << ", ";
	}
    }
}
