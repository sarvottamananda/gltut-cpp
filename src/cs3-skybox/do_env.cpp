/*
    Sarvottamananda (shreesh)
    2020-09-20
    do_env.cpp v0.0 (OpenGL Code Snippets)

    Process environment variables
*/

#include "do_env.h"

#include <vector>

void process_env(Opts& os)
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

    if (vallist[0] != nullptr) os.assetspath = vallist[0];
    if (vallist[1] != nullptr) os.searchpath = vallist[1];
    if (vallist[2] != nullptr) os.modelsdir = vallist[2];
    if (vallist[3] != nullptr) os.shadersdir = vallist[3];
    if (vallist[4] != nullptr) os.texturesdir = vallist[4];
    if (vallist[5] != nullptr) os.homedir = vallist[5];
}
