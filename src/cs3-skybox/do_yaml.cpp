// Project:		OpenGL Code Snippets
// File:		do_yaml.cpp
// Version:		0.0
// Last changed:	2020-09-19
// Purpose:		Code for the debugging system
// Author:		shreesh
// Credits:
// License:		MIT
// Copyright:		(C) Sarvottamananda  2020

#include "do_yaml.h"

#include <yaml-cpp/yaml.h>

#include <iostream>

#include "debug.h"

void process_yaml(std::string conffile, Options_store& os)
{
    try {
	YAML::Node config = YAML::LoadFile(conffile);
	if (config) {
	    if (auto val = config["assets_path"]) {
		os.set_assetspath(val.as<std::string>());
	    }
	    if (auto val = config["build_dir"]) {
		os.set_builddir(val.as<std::string>());
	    }
	    if (auto val = config["source_dir"]) {
		os.set_sourcedir(val.as<std::string>());
	    }
	}
    }
    catch (YAML::BadFile& e) {
	std::cerr << "Bad yaml file : (" << conffile << ")\n";
	return;
    }
}
