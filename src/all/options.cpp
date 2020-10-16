//	Sarvottamananda (shreesh)
//	2020-09-28
//	options.cpp v0.0 (OpenGL Code Snippets)
//      Opt class implementation

#include "options.h"

#include <getopt.h>
#include <yaml-cpp/yaml.h>

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static int getnumarg(char*);
static void print_usage(char*);
static void process_build(Options* os);
static void process_yaml(std::string conffile, Options* os);
static void process_env(Options* os);

Options::Options()
    : verbose(false),
      debug(0),
      fullscreen(false),
      height(600),
      width(800),
      configfile("conf.yml"),
      assetspath("./assets"),
      searchpath("..:."),
      modelsdir("models"),
      shadersdir("shaders"),
      texturesdir("textures"),
      builddir("."),
      sourcedir("."),
      homedir("~"),
      configdir("config")
{
    return;
}

void
Options::print(std::string name)
{
    printf(
	"Options (%s) {\n"
	"\tfullscreen = %d\n"
	"\tverbose = %d\n"
	"\tdebug = %d\n"
	"\theight = %d\n"
	"\twidth = %d\n"
	"\tconfigfile = %s\n"
	"\tassetspath = %s\n"
	"\tsearchpath = %s\n"
	"\tmodelsdir = %s\n"
	"\tshadersdir = %s\n"
	"\ttexturesdir = %s\n"
	"\tbuilddir = %s\n"
	"\tsourcedir = %s\n"
	"\thomedir = %s\n"
	"\tconfigdir = %s\n"
	"}\n",
	name.c_str(), fullscreen, verbose, debug, height, width, configfile.c_str(),
	assetspath.c_str(), searchpath.c_str(), modelsdir.c_str(), shadersdir.c_str(),
	texturesdir.c_str(), builddir.c_str(), sourcedir.c_str(), homedir.c_str(),
	configdir.c_str());
    fflush(stdout);
}

void
Options::process_options(int argc, char** argv)
{
    int res = -1;

    bool verbose = false;
    int debug = 0;
    bool fullscreen = false;
    int height = 0;
    int width = 0;
    std::string conffile = configfile;

    bool has_vopt = false;
    bool has_dopt = false;
    bool has_copt = false;
    bool has_fopt = false;
    bool has_hopt = false;
    bool has_wopt = false;

    struct option long_options[] = {
	{"brief", no_argument, nullptr, 0},	    // 0
	{"fullscreen", no_argument, nullptr, 0},    // 1
	{"windowed", no_argument, nullptr, 0},	    // 2
	{"width", required_argument, nullptr, 0},   // 3
	{"height", required_argument, nullptr, 0},  // 4

	{"help", no_argument, nullptr, 'h'},	     // 5
	{"verbose", no_argument, nullptr, 'v'},	     // 7
	{"debug", required_argument, nullptr, 'd'},  // 6
	{"config", no_argument, nullptr, 'c'},	     // 8
	{0, 0, 0, 0},
    };
    std::string short_options = "hvd:c:";
    int long_index = 0;

    while (1) {
	res = getopt_long(argc, argv, short_options.c_str(), long_options, &long_index);

	if (res == 0) {
	    // nothing to do if it is fullscreen  or windowed
	    if (strcmp(long_options[long_index].name, "brief") == 0) {
		verbose = false;
		has_vopt = true;
	    }
	    if (strcmp(long_options[long_index].name, "fullscreen") == 0) {
		fullscreen = true;
		has_fopt = true;
	    }
	    if (strcmp(long_options[long_index].name, "windowed") == 0) {
		fullscreen = false;
		has_fopt = true;
	    }
	    if (strcmp(long_options[long_index].name, "height") == 0) {
		height = getnumarg(optarg);
		has_hopt = true;
	    }
	    if (strcmp(long_options[long_index].name, "width") == 0) {
		width = getnumarg(optarg);
		has_wopt = true;
	    }
	    continue;
	}

	if (res == -1) break;

	switch (res) {
	    case 'v':
		verbose = true;
		has_vopt = true;
		break;
	    case 'd':
		debug = getnumarg(optarg);
		has_dopt = true;
		break;
	    case 'h':
		print_usage(argv[0]);
		break;
	    case 'c':
		conffile = optarg;
		break;
	    case '?':
		// getopt_long already printed an error message, but we print the usage
		// nevertheless
		print_usage(argv[0]);
		break;

	    default:
		print_usage(argv[0]);
		abort();
	}
    }

    // Print any remaining command line arguments (not options).
    if (optind < argc) {
	fprintf(stderr, "non-option argv-elements: ");
	while (optind < argc) fprintf(stderr, "%s ", argv[optind++]);
	fputc('\n', stderr);
    }

    // First process the configuration file for the options, because the environment vars and
    // then command line options overide the configuration file.

    if (has_copt) {
	this->configfile = conffile;
    }
    process_yaml(this->configdir + "/" + this->configfile, this);

    process_env(this);

    if (has_vopt) this->verbose = verbose;
    if (has_dopt) this->debug = debug;
    if (has_fopt) this->fullscreen = fullscreen;
    if (has_hopt) this->height = height;
    if (has_wopt) this->width = width;
    return;
}

void
Options::process_build(const char bdir[], const char sdir[], const char cdir[], const char cfile[])
{
    builddir = bdir;
    sourcedir = sdir;
    configdir = cdir;
    configfile = cfile;
}

static int
getnumarg(char* s)
{
    try {
	std::size_t pos;
	int num = std::stoi(s, &pos);
	if (pos < strlen(s)) {
	    fprintf(stderr, "Trailing characters after number: %s\n", s);
	}
	return num;
    }
    catch (const std::invalid_argument& ex) {
	fprintf(stderr, "Invalid number: %s\n", ex.what());
    }
    catch (const std::out_of_range& ex) {
	fprintf(stderr, "Number out of range: %s\n", ex.what());
    }
    abort();
}

static void
print_usage(char* program_name)
{
    printf(
	"Usage: %s <options>\n"
	"Options:\n"
	"\t-h,--help\t\tShow this help\n"
	"\t-v,--verbose\tVerbose output\n"
	"\t-d,--debug level\tOutput debug/trace messages if the program was built with debug "
	"\t-c,--config config_file\tConfiguration file"
	"support\n"
	"\t--brief\tTerse output\n"
	"\t--fullscreen\t\tProgram runs in fullscreen\n"
	"\t--windowed\t\tProgram runs in a window (default)\n"
	"\t--height\t\tHeight of the window otherwise it has default height\n"
	"\t--width\t\tWidth of the window otherwise it has default width\n",
	program_name);
}

void
process_yaml(std::string conffile, Options* os)
{
    try {
	YAML::Node config = YAML::LoadFile(conffile);
	if (config) {
	    if (auto val = config["assets_path"]) {
		os->assetspath = val.as<std::string>();
	    }
	    if (auto val = config["build_dir"]) {
		os->builddir = val.as<std::string>();
	    }
	    if (auto val = config["source_dir"]) {
		os->sourcedir = val.as<std::string>();
	    }
	}
    }
    catch (YAML::BadFile& e) {
	std::cerr << "Bad yaml file : " << e.what() << " (" << conffile.c_str() << ")\n";
	return;
    }
}

void
process_env(Options* os)
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

    if (vallist[0] != nullptr) os->assetspath = vallist[0];
    if (vallist[1] != nullptr) os->searchpath = vallist[1];
    if (vallist[2] != nullptr) os->modelsdir = vallist[2];
    if (vallist[3] != nullptr) os->shadersdir = vallist[3];
    if (vallist[4] != nullptr) os->texturesdir = vallist[4];
    if (vallist[5] != nullptr) os->homedir = vallist[5];
}
