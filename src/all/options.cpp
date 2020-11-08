//	Sarvottamananda (shreesh)
//	2020-09-28
//	options.cpp v0.0 (OpenGL Code Snippets)
//      Opt class implementation

#include "options.h"

#include <getopt.h>

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static int getnumarg(char*);
static void print_usage(char*);

Options::Options() : verbose(false), debug(0), fullscreen(false), height(600), width(800)
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
        "}\n",
        name.c_str(), fullscreen, verbose, debug, height, width);
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

    bool has_vopt = false;
    bool has_dopt = false;
    bool has_fopt = false;
    bool has_hopt = false;
    bool has_wopt = false;

    struct option long_options[] = {
        {"brief", no_argument, nullptr, 0},         // 0
        {"fullscreen", no_argument, nullptr, 0},    // 1
        {"windowed", no_argument, nullptr, 0},      // 2
        {"width", required_argument, nullptr, 0},   // 3
        {"height", required_argument, nullptr, 0},  // 4

        {"help", no_argument, nullptr, 'h'},         // 5
        {"debug", required_argument, nullptr, 'd'},  // 6
        {"verbose", no_argument, nullptr, 'v'},      // 7
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

    if (has_vopt) this->verbose = verbose;
    if (has_dopt) this->debug = debug;
    if (has_fopt) this->fullscreen = fullscreen;
    if (has_hopt) this->height = height;
    if (has_wopt) this->width = width;
    return;
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
    exit(EXIT_SUCCESS);
}
