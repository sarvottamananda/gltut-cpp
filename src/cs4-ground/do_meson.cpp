// Sarvottamananda (shreesh)
// 2020-09-28
// do_meson.cpp v0.0 (OpenGL Code Snippets)
//
// Process build configuration variables

#include "do_meson.hpp"

#include "cs_config.hpp"

void process_buildconf(Options_store& os)
{
    // Currently we are directly setting the builddir and srcdir variable in the config.h using
    // meson build system.

    // os.set_builddir(cs_config::cs_build_dir);
    // os.set_sourcedir(cs_config::cs_source_dir);
}
