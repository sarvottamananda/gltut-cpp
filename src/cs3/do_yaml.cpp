// Sarvottamananda (shreesh)
// 2020-09-28
// do_yaml.cpp v0.0 (OpenGL Code Snippets)
//
// Process configuration file

#include "do_yaml.h"

#include <yaml-cpp/yaml.h>

#include <iostream>

#include "options_store.h"

void process_yaml(std::string conffile, Options_store& os)
{
    try {
	YAML::Node config = YAML::LoadFile(conffile);
	if (config) {
	    if (auto val = config["assets_path"]) {
		os.assetspath = val.as<std::string>();
	    }
	    if (auto val = config["build_dir"]) {
		os.builddir = val.as<std::string>();
	    }
	    if (auto val = config["source_dir"]) {
		os.sourcedir = val.as<std::string>();
	    }
	}
    }
    catch (YAML::BadFile& e) {
	std::cerr << "Bad yaml file : " << e.what() << " (" << conffile.c_str() << ")\n";
	return;
    }
}
