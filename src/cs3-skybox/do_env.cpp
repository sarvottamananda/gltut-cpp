// Project:		OpenGL Code Snippets
// File:		do_env.cpp
// Version:		0.0
// Last changed:	2020-09-19
// Purpose:		Code for the debugging system
// Author:		shreesh
// Credits:
// License:		MIT
// Copyright:		(C) Sarvottamananda  2020

#include "do_env.h"

#include <cstdlib>
#include <vector>

#include "debug.h"

void process_env(Options_store& os)
{
    std::vector<std::string> namelist{
	"CS_ASSETS_PATH", "CS_SEARCH_PATH",  "CS_MODELS_DIR",
	"CS_SHADERS_DIR", "CS_TEXTURES_DIR", "HOME",
    };

    std::vector<char*> vallist(namelist.size());

    auto i = 0;
    for (auto s : namelist) {
	vallist[i++] = std::getenv(s.c_str());
    }

    if (vallist[0] != nullptr) os.set_assetspath(std::string(vallist[0]));
    if (vallist[1] != nullptr) os.set_searchpath(std::string(vallist[1]));
    if (vallist[2] != nullptr) os.set_modelsdir(std::string(vallist[2]));
    if (vallist[3] != nullptr) os.set_shadersdir(std::string(vallist[3]));
    if (vallist[4] != nullptr) os.set_texturesdir(std::string(vallist[4]));
}
