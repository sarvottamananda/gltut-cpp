// Sarvottamananda (shreesh)
// 2020-09-28
// do_args.cpp v0.0 (OpenGL Code Snippets)
//
// Process args, env, conf file and build vars.

#include "do_args.h"

#include <getopt.h>

#include <cstring>
#include <stdexcept>
#include <string>

#include "cs_config.h"
#include "do_env.h"
#include "do_meson.h"
#include "do_yaml.h"
#include "options_store.h"

static int getnumarg(char *);
static void print_usage(char *);

void process_args(int argc, char **argv, Options_store &os)
{
    int res = -1;

    bool verbose = false;
    int debug = 0;
    bool fullscreen = false;
    int height = 0;
    int width = 0;
    std::string conffile = cs_config::cs_config_file;

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
		verbose = 1;
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

    process_buildconf(os);

    if (has_copt) {
	os.configfile = conffile;
    }
    process_yaml(os.configfile, os);

    process_env(os);

    if (has_vopt) os.verbose = verbose;
    if (has_dopt) os.debug = debug;
    if (has_fopt) os.fullscreen = fullscreen;
    if (has_hopt) os.height = height;
    if (has_wopt) os.width = width;
    return;
}

static int getnumarg(char *s)
{
    try {
	std::size_t pos;
	int num = std::stoi(s, &pos);
	if (pos < strlen(s)) {
	    fprintf(stderr, "Trailing characters after number: %s\n", s);
	}
	return num;
    }
    catch (const std::invalid_argument &ex) {
	fprintf(stderr, "Invalid number: %s\n", ex.what());
    }
    catch (const std::out_of_range &ex) {
	fprintf(stderr, "Number out of range: %s\n", ex.what());
    }
    abort();
}

static void print_usage(char *program_name)
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
